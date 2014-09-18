#ifndef SC_COORDINATE_H
#define SC_COORDINATE_H

#include <boost/functional/hash/extensions.hpp>
#include <boost/operators.hpp>
#include <functional>
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

namespace std
{

template<>
struct hash<StellarCartography::Coordinate>
{
    typedef StellarCartography::Coordinate argument_type;
    typedef hash<double>::result_type result_type;

    inline result_type 
    operator()(const argument_type& s) const
    {
        hash<double> h;
        return h(s.x()) ^ (h(s.y()) << 1) ^ (h(s.z()) << 2);
    }
};

}

namespace boost
{

template<>
struct hash<StellarCartography::Coordinate> : 
    public std::hash<StellarCartography::Coordinate>
{
};

}

#endif /* SC_COORDINATE_H */
