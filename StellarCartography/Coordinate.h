#ifndef SC_COORDINATE_H
#define SC_COORDINATE_H

#include <boost/operators.hpp>
#include <tuple>

namespace StellarCartography
{

class Coordinate : 
    boost::totally_ordered<Coordinate>
{
    double v[3];

public:
    Coordinate();
    Coordinate(double x, double y, double z);

    double x() const { return v[0]; }
    double y() const { return v[1]; }
    double z() const { return v[2]; }

    double distanceSquared(const Coordinate& other) const;
    double distance(const Coordinate& other) const;

    bool operator==(const Coordinate& other) const;
    bool operator<(const Coordinate& other) const;
    int compare(const Coordinate& otheer) const;
};

} /* StellarCartography */

#endif /* SC_COORDINATE_H */
