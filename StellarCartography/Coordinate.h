#ifndef SC_COORDINATE_H
#define SC_COORDINATE_H

#include <tuple>

namespace StellarCartography
{

class Coordinate
{
    double x_;
    double y_;
    double z_;

public:
    Coordinate();
    Coordinate(double x, double y, double z);

    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    double distanceSquared(const Coordinate& other) const;
    double distance(const Coordinate& other) const;

    bool operator==(const Coordinate& other) const;
};

} /* StellarCartography */

#endif /* SC_COORDINATE_H */
