#include "StellarCartography/Coordinate.h"

#include <cmath>

namespace StellarCartography
{

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
    double sum = 0;
    for (int i = 0; i < 3; ++i)
    {
        double n = v[i] - o.v[i];
        sum += n*n;
    }
    return sum;
}

double Coordinate::distance(const Coordinate& o) const
{
    return std::sqrt(distanceSquared(o));
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

}
