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
    std::map<std::string, Star> names;

public:
    SpatialIndex();

    void insert(const Star& s);
    bool contains(const Star& s) const;
    size_t size() const;

    Star nearestNeighbor(const std::string& name, double threshold) const;
    Star nearestNeighbor(const Star& star, double threshold) const;

    StarSet neighbors(const std::string& name, double threshold) const;
    StarSet neighbors(const Star& star, double threshold) const;

    StarList path(const std::string& name, double threshold) const;
    StarList path(const Star& star, double threshold) const;

    StarSet reachable(const std::string& name, double threshold) const;
    StarSet reachable(const Star& star, double threshold) const;

    std::list<StarSet> connectedComponents(double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_SPATIAL_INDEX_H */
