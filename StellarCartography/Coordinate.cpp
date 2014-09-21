#include "StellarCartography/Coordinate.h"

#include <boost/concept_check.hpp>
#include <cmath>
#include <numeric>

using namespace StellarCartography;
using namespace boost;

namespace 
{

void concept_check()
{
    BOOST_CONCEPT_ASSERT((Assignable<Coordinate>));
    BOOST_CONCEPT_ASSERT((DefaultConstructible<Coordinate>));
    BOOST_CONCEPT_ASSERT((CopyConstructible<Coordinate>));
    BOOST_CONCEPT_ASSERT((EqualityComparable<Coordinate>));
    BOOST_CONCEPT_ASSERT((LessThanComparable<Coordinate>));

    Coordinate { 0, 0, 0 };
}

} /* namespace */

Coordinate::Coordinate() :
    v { 0, 0, 0 }
{
}

Coordinate::Coordinate(double x, double y, double z) : 
    v { x, y, z }
{
}

double Coordinate::distanceSquared(const Coordinate& o) const
{
    return std::inner_product(
        data(), data() + 3, o.data(), 0.0, 
        std::plus<double>(),
        [](double l, double r)
        {
            return (l - r) * (l - r);
        }
    );
}

double Coordinate::distance(const Coordinate& o) const
{
    return std::sqrt(distanceSquared(o));
}

double Coordinate::manhattanDistance(const Coordinate& o) const
{
    return std::inner_product(
        data(), data() + 3, o.data(), 0.0,
        std::plus<double>(), 
        [](double l, double r)
        {
            return std::abs(l - r);
        }
    );
}

bool Coordinate::operator==(const Coordinate& o) const
{
    return compare(o) == 0;
}

bool Coordinate::operator<(const Coordinate& o) const
{
    return compare(o) < 0;
}

int Coordinate::compare(const Coordinate& o) const
{
    for (int i = 0; i < 3; ++i)
    {
        if (v[i] < o.v[i]) return -1;
        if (v[i] > o.v[i]) return 1;
    }
    return 0;
}
