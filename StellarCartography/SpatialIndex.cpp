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
SpatialIndex::nearestNeighbor(const std::string& name, double threshold) const
{
    return Star();
}

std::list<Star> 
SpatialIndex::neighbors(const std::string& name, double threshold) const
{
    return { };
}

std::list<Star> 
SpatialIndex::path(const std::string& name, double threshold) const
{
    return { };
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
