#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

SpatialIndex::SpatialIndex()
{
}

NeighborMap SpatialIndex::getNeighborMap(double t, double tolerance) const
{
    auto it = maps.upper_bound(t);

    if (it != maps.end() &&
        (it->first - t) >= 0.0 && (it->first - t) < tolerance)
    {
        return it->second;
    }

    NeighborMap result(t);
    
    for (auto kv : names)
    {
        result.add(kv.second);
    }
    maps.emplace(t, std::move(result));
    return result;
}

void SpatialIndex::insert(const Star& s)
{ 
    for (auto kv : maps)
    {
        kv.second.add(s);
    }
    names.emplace(s.getName(), std::move(s));
}

Star
SpatialIndex::nearestNeighbor(const Star& star, double threshold) const
{
    return nearestNeighbor(star.getName(), threshold);
}

Star 
SpatialIndex::nearestNeighbor(const std::string& name, double threshold) const
{
    return Star();
}

StarSet 
SpatialIndex::neighbors(const Star& star, double threshold) const
{
    return neighbors(star.getName(), threshold);
}

StarSet 
SpatialIndex::neighbors(const std::string& name, double threshold) const
{
    return { };
}

StarList 
SpatialIndex::path(const Star& from, const Star& to, double threshold) const
{
    return path(from.getName(), to.getName(), threshold);
}

StarList 
SpatialIndex::path(
    const std::string& from, 
    const std::string& to,
    double threshold) const
{
    return { };
}

StarSet 
SpatialIndex::reachable(const Star& star, double threshold) const
{
    return reachable(star.getName(), threshold);
}

StarSet 
SpatialIndex::reachable(const std::string& name, double threshold) const
{
    return { };
}

std::list<StarSet> 
SpatialIndex::connectedComponents(double threshold) const
{
    return { };
}
