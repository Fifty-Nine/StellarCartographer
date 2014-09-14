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

std::list<Star> 
SpatialIndex::neighbors(const Star& star, double threshold) const
{
    return neighbors(star.getName(), threshold);
}

std::list<Star> 
SpatialIndex::neighbors(const std::string& name, double threshold) const
{
    return { };
}

std::list<Star> 
SpatialIndex::path(const Star& star, double threshold) const
{
    return path(star.getName(), threshold);
}

std::list<Star> 
SpatialIndex::path(const std::string& name, double threshold) const
{
    return { };
}

std::set<Star> 
SpatialIndex::reachable(const Star& star, double threshold) const
{
    return reachable(star.getName(), threshold);
}

std::set<Star> 
SpatialIndex::reachable(const std::string& name, double threshold) const
{
    return { };
}

std::list<std::set<Star>> 
SpatialIndex::connectedComponents(double threshold) const
{
    return { };
}
