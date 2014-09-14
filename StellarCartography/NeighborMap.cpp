#include "StellarCartography/NeighborMap.h"

using namespace StellarCartography;

NeighborMap::NeighborMap(double threshold) : 
    t2_(threshold * threshold)
{
}

void NeighborMap::add(const Star& s)
{
}

void NeighborMap::remove(const Star& s)
{
}

std::list<Star> 
NeighborMap::neighbors(const std::string& name, double threshold) const
{
    return { };
}

Star 
NeighborMap::nearestNeighbor(const std::string& name, double threshold) const
{
    return Star();
}
