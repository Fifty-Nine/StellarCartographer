#ifndef SC_NEIGHBOR_MAP
#define SC_NEIGHBOR_MAP

#include <list>
#include <map>
#include <unordered_map>
#include <string>

#include "StellarCartography/Star.h"

namespace boost
{
template<class T>
struct graph_traits;
}

namespace StellarCartography
{

class NeighborMap
{
    typedef std::multimap<double, Star> DistanceMap;
    typedef DistanceMap::value_type MapEntry;
    typedef std::unordered_map<Star, DistanceMap> NeighborTable;
    typedef NeighborTable::value_type TableEntry;

    double t2_;
    NeighborTable nodes_;

    DistanceMap get(const Star& star) const;
    double checkThreshold(double t) const;

    StarSet neighborsImpl(const Star& name, double t2) const;
    Star nearestNeighborImpl(const Star& name, double t2) const;

public:
    NeighborMap(double threshold);

    friend struct boost::graph_traits<NeighborMap>;

    void add(const Star& s);
    StarSet neighbors(const Star& name) const;
    StarSet neighbors(const Star& name, double threshold) const;
    Star nearestNeighbor(const Star& name) const;
    Star nearestNeighbor(const Star& name, double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_NEIGHBOR_MAP */
