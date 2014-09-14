#include "StellarCartography/NeighborMap.h"

using namespace StellarCartography;

NeighborMap::NeighborMap(double threshold) : 
    t2_(threshold * threshold)
{
}

void NeighborMap::add(const Star& s)
{
    DistanceMap neighbors;
    for (auto kv : nodes_)
    {
        auto t = kv.first;
        double d2 = t.getCoords().distanceSquared(s.getCoords());

        if (d2 < t2_)
        {
            neighbors.emplace(MapEntry { d2, t });
            nodes_[t].emplace(MapEntry { d2, s });
        }
    }

    nodes_.emplace(TableEntry { s, std::move(neighbors) });
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
