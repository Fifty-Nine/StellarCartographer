#ifndef SC_SPATIAL_INDEX_H
#define SC_SPATIAL_INDEX_H

#include <list>
#include <map>
#include <set>

#include "StellarCartography/NeighborMap.h"
#include "StellarCartography/Star.h"

namespace StellarCartography
{

class Coordinate;

class SpatialIndex
{
    typedef std::map<std::string, Star> NameMap;
    typedef NameMap::value_type NameEntry;
    typedef std::map<double, NeighborMap> LookupTable;
    typedef LookupTable::value_type LookupEntry;
    NameMap names;
    mutable LookupTable maps;

    NeighborMap getNeighborMap(double t, double tolerance = 0.5) const;

public:
    SpatialIndex();

    void insert(const Star& s);

    Star nearestNeighbor(const std::string& name, double threshold) const;
    Star nearestNeighbor(const Star& star, double threshold) const;

    StarSet neighbors(const std::string& name, double threshold) const;
    StarSet neighbors(const Star& star, double threshold) const;

    StarList path(
        const std::string& from, 
        const std::string& to, 
        double threshold) const;
    StarList path(
        const Star& from, 
        const Star& to, 
        double threshold) const;

    StarSet reachable(const std::string& name, double threshold) const;
    StarSet reachable(const Star& star, double threshold) const;

    std::list<StarSet> connectedComponents(double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_SPATIAL_INDEX_H */
