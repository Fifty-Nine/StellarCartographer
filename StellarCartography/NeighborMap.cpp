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

StarSet
NeighborMap::neighbors(const Star& star, double threshold) const
{
    return { };
}

Star 
NeighborMap::nearestNeighbor(const Star& star, double threshold) const
{
    return Star();
}
