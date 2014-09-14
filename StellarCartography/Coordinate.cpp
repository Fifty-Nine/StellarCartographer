#include "StellarCartography/Coordinate.h"

#include <cmath>

namespace StellarCartography
{

Coordinate::Coordinate() :
    x_(0.0), y_(0.0), z_(0.0)
{
}

Coordinate::Coordinate(double x, double y, double z) : 
    x_(x), y_(y), z_(z)
{
}

double Coordinate::distanceSquared(const Coordinate& o) const
{
    double x = x_ - o.x_;
    double y = y_ - o.y_;
    double z = z_ - o.z_;
    return x*x + y*y + z*z;
}

double Coordinate::distance(const Coordinate& o) const
{
    return std::sqrt(distanceSquared(o));
}

bool Coordinate::operator==(const Coordinate& o) const
{
    return x_ == o.x_ && y_ == o.y_ && z_ == o.z_;
}

}
