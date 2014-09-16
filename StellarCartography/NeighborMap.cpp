#include "StellarCartography/NeighborMap.h"

#include <algorithm>
#include <boost/graph/graph_concepts.hpp>
#include <boost/concept_check.hpp>
#include <cmath>
#include <sstream>

#include "StellarCartography/NeighborMapGraphTraits.h"

using namespace StellarCartography;

#define sq(x) ((x)*(x))

namespace StellarCartography
{

typedef boost::graph_traits<NeighborMap> traits;


} /* namespace StellarCartography */

NeighborMap::NeighborMap(double threshold) : 
    t2_(sq(threshold))
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<NeighborMap>));
    BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<NeighborMap>));
    BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<NeighborMap>));
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
NeighborMap::neighborsImpl(const Star& star, double t2) const
{
    auto nearby = get(star);

    StarSet result;
    std::transform(
        nearby.begin(),
        nearby.upper_bound(t2),
        inserter(result, result.begin()),
        [](const MapEntry& e) { return e.second; }
    );
    return result;
}

Star 
NeighborMap::nearestNeighborImpl(const Star& star, double t2) const
{
    auto nearby = get(star);

    if (nearby.size() == 0) { return Star(); }

    auto nearest = nearby.begin();

    return (nearest->first < t2) ? nearest->second : Star();
}

StarSet NeighborMap::neighbors(const Star& name) const 
{
    return neighborsImpl(name, t2_);
}

StarSet NeighborMap::neighbors(const Star& name, double t) const
{
    return neighborsImpl(name, checkThreshold(t));
}

Star NeighborMap::nearestNeighbor(const Star& name) const
{
    return nearestNeighborImpl(name, t2_);
}

Star NeighborMap::nearestNeighbor(const Star& name, double t) const
{
    return nearestNeighborImpl(name, checkThreshold(t));
}

double NeighborMap::checkThreshold(double threshold) const
{
    auto t2 = sq(threshold);

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

    return t2;
}

NeighborMap::DistanceMap NeighborMap::get(const Star& star) const
{
    auto it = nodes_.find(star);
    if (it == nodes_.end())
    {
        std::ostringstream os;
        os << "Unknown star: " << star.getName();
        throw std::invalid_argument(os.str());
    }

    return it->second;
}
