#ifndef SC_NEIGHBOR_MAP
#define SC_NEIGHBOR_MAP

#include <list>
#include <map>
#include <string>

#include "StellarCartography/Star.h"

namespace StellarCartography
{

class NeighborMap
{
    typedef std::multimap<double, Star> DistanceMap;
    typedef DistanceMap::value_type MapEntry;
    typedef std::map<Star, DistanceMap> NeighborTable;
    typedef NeighborTable::value_type TableEntry;

    double t2_;
    NeighborTable nodes_;

    DistanceMap get(const Star& star) const;
    double checkThreshold(double t) const;

public:
    NeighborMap(double threshold);

    void add(const Star& s);
    StarSet neighbors(const Star& name, double threshold) const;
    Star nearestNeighbor(const Star& name, double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_NEIGHBOR_MAP */
