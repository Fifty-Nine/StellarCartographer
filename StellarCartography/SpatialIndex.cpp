#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

SpatialIndex::SpatialIndex()
{
}

void SpatialIndex::insert(const Star& s)
{
}

bool SpatialIndex::contains(const Star& s) const
{
    return false;
}

size_t SpatialIndex::size() const
{
    return 0;
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
