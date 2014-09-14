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

    std::list<Star> neighbors(const std::string& name, double threshold) const;
    std::list<Star> neighbors(const Star& star, double threshold) const;

    std::list<Star> path(const std::string& name, double threshold) const;
    std::list<Star> path(const Star& star, double threshold) const;

    std::set<Star> reachable(const std::string& name, double threshold) const;
    std::set<Star> reachable(const Star& star, double threshold) const;

    std::list<std::set<Star>> connectedComponents(double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_SPATIAL_INDEX_H */
