#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

SpatialIndex::SpatialIndex()
{
}

Star SpatialIndex::getStar(const std::string& name) const
{
    auto it = names.find(name);
    if (it == names.end())
    {
        throw std::invalid_argument(name);
    }

    return it->second;
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
    return { }; 
}

Star 
SpatialIndex::nearestNeighbor(const std::string& name, double threshold) const
{
    return nearestNeighbor(getStar(name), threshold);
}

StarSet 
SpatialIndex::neighbors(const Star& star, double threshold) const
{
    return { }; 
}

StarSet 
SpatialIndex::neighbors(const std::string& name, double threshold) const
{
    return neighbors(getStar(name), threshold);
}

StarList 
SpatialIndex::path(const Star& from, const Star& to, double threshold) const
{
    return { };
}

StarList 
SpatialIndex::path(
    const std::string& from, 
    const std::string& to,
    double threshold) const
{
    return path(getStar(from), getStar(to), threshold);
}

StarSet 
SpatialIndex::reachable(const Star& star, double threshold) const
{
    return { };
}

StarSet 
SpatialIndex::reachable(const std::string& name, double threshold) const
{
    return reachable(getStar(name), threshold);
}

std::list<StarSet> 
SpatialIndex::connectedComponents(double threshold) const
{
    return { };
}
