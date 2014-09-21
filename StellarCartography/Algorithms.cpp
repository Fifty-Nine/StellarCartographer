#include "Algorithms.h"

#include <boost/iterator/transform_iterator.hpp>
#include <cmath>
#include <combinations_iterator.hpp>
#include <eigen3/Eigen/Dense>

using namespace StellarCartography;
using namespace StellarCartography::Detail;
using namespace Eigen;
using namespace boost;
using namespace std;

namespace
{

Vector3d toVector(const Coordinate& p)
{
    return { p.x(), p.y(), p.z() };
}

Coordinate fromVector(const Vector3d& v)
{
    return { v.x(), v.y(), v.z() };
}

double error(const Coordinate& candidate, const SampleList& l)
{
    typedef std::function<double(Sample)> ErrorFcn;
    ErrorFcn efn = [candidate](const Sample& s)
    {
        return std::abs(s.first.distance(candidate) - s.second);
    };

    auto begin = make_transform_iterator(l.begin(), efn);
    auto end = make_transform_iterator(l.end(), efn);

    return std::accumulate(begin, end, 0.0);
}

}

Solution StellarCartography::Detail::trilaterateOne(
    const Sample& s1, const Sample& s2, const Sample& s3)
{
    /* 
     * Trilaterate the position of a point given three known locations and
     * the distances from them. The approach used here is to transform the 
     * given coordinates into a new coordinate system with P1 at the origin, 
     * P2 somewhere on the x-axis and P3 somewhere in the x-y plane. We 
     * compute the positions of P2 and P3 in our new coordinate system, then 
     * solve for the unknown position algebraically. 
     *
     * P1 is at the origin, so:
     * r1^2 = x'^2 + y'^2 + z'^2
     *
     * P2 is at distance d from P1 on the x'-axis, so its coordinates are
     * (d, 0, 0), and:
     * r2^2 = (x' - d)^2 + y'^2 + z'^2
     *
     * P3 is more tricky. The x coordinate is given by projecting the vector
     * P1->P3 onto the x-axis--call it 'i'. Likewise, The y coordinate is the 
     * projection of P1->P3 onto the y-axis, called 'j', so:
     * r3^2 = (x' - i)^2 + (y' - j)^2 + z'^2
     *
     * We can solve for x by subtracting the first two equations and doing some 
     * algebra:
     * x' = (r1^2 - r2^2 + d^2)/(2*d)
     *
     * Substituting back into the first equation, and ~=*more algebra*=~:
     * y' = (r1^2 - r3^2 + i^2 + j^2) / (2*j) - (i / j) * x'
     *
     * Finally, solving the first equation for z:
     * z' = sqrt(r1^2 - x'^2 - y'^2)
     *
     * This can be positive or negative, but only one solution will satisfy all
     * the equations. Once we have 
     */
    auto p1 = toVector(s1.first);
    auto p2 = toVector(s2.first);
    auto p3 = toVector(s3.first);
    auto r1 = s1.second;
    auto r2 = s2.second;
    auto r3 = s3.second;

    /* Vectors pointing at P2 and P3 from P1. */
    auto p2p = p2 - p1;
    auto p3p = p3 - p1;

    auto ez = p2p.cross(p3p).normalized();
    auto ex = p2p.normalized();
    auto ey = ez.cross(ex).normalized();

    /* x-coordinate of P2 */
    auto d = p2p.norm();
    
    /* x, y coordinates of P3 */
    auto i = ex.dot(p3p);
    auto j = ey.dot(p3p);

    /* Square some things. */
    auto r1_2 = r1 * r1;
    auto r2_2 = r2 * r2;
    auto r3_2 = r3 * r3;
    auto d_2 = d * d;
    auto i_2 = i * i;
    auto j_2 = j * j;

    /* Solve for x' */
    auto xp = (r1_2 - r2_2 + d_2) / (2 * d);
    auto xp_2 = xp * xp;

    /* Solve for y' */
    auto yp = (r1_2 - r3_2 + i_2 + j_2) / (2 * j) - (i / j) * xp;
    auto yp_2 = yp * yp;

    /* Solve for z' */
    auto zp = sqrt(r1_2 - xp_2 - yp_2);
    
    auto xy = p1 + xp * ex + yp * ey;
    auto z = zp * ez;

    return Solution(fromVector(xy + z), fromVector(xy - z));
}

Coordinate StellarCartography::Detail::trilaterateMany(
    const SampleList& samples)
{
    if (samples.size() < 3) 
        throw std::length_error("Not enough samples to trilaterate");

    auto begin_c = make_combinations_iterator<3>(samples.begin(), samples.end());
    auto end_c = make_combinations_iterator<3>(samples.end(), samples.end());

    typedef std::pair<double, Coordinate> Result;
    typedef std::function<Result(std::array<Sample, 3>)> Xformer;

    Xformer xfrm = [&samples](const std::array<Sample, 3>& c)
    {
        auto s = trilaterateOne(c[0], c[1], c[2]);

        double e0 = error(s.first, samples);
        double e1 = error(s.second, samples);

        return (e1 < e0) ? Result(e1, s.second) : Result(e0, s.first);
    };

    auto begin = make_transform_iterator(begin_c, xfrm);
    auto end = make_transform_iterator(end_c, xfrm);

    return std::min_element(begin, end)->second;
}

