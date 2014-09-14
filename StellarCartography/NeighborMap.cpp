#include "StellarCartography/NeighborMap.h"

#include <cmath>

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
    checkThreshold(threshold);
    return { };
}

Star 
NeighborMap::nearestNeighbor(const Star& star, double threshold) const
{
    checkThreshold(threshold);

    auto it = nodes_.find(star);
    if (it == nodes_.end())
    {
        throw std::invalid_argument("star");
    }

    auto nearby = it->second;
    if (nearby.size() == 0) { return Star(); }

    auto nearest = nearby.begin();
    double t2 = threshold * threshold;

    return (nearest->first < t2) ? nearest->second : Star();
}

void NeighborMap::checkThreshold(double threshold) const
{
    auto t2 = threshold;
    t2 *= t2;

    if (isnan(threshold))
    {
        throw std::domain_error("threshold not a number");
    }
    else if (threshold < 0.0)
    {
        throw std::domain_error("threshold less than 0");
    }
    else if (t2 > t2_)
    {
        throw std::out_of_range("threshold too large");
    }
}
