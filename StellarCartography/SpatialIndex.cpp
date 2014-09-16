#include "StellarCartography/SpatialIndex.h"

#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <limits>
#include <sstream>

#include "StellarCartography/NeighborMapGraphTraits.h"

using namespace StellarCartography;

namespace
{

template<class EventFilter>
class lambda_visitor
{
    typedef Star T;
    typedef NeighborMap G;
    typedef std::function<void(const T&, const G&)> fcn_type;
    fcn_type f_;
public:
    typedef EventFilter event_filter;
    lambda_visitor(const fcn_type& f = fcn_type()) : f_(f) { }

    void operator()(const T& t, const G& g) { f_(t, g); }
};

template<class T>
using StarMap = std::unordered_map<Star, T>;
template<class T>
using PMap = boost::associative_property_map<StarMap<T>>;

}

SpatialIndex::SpatialIndex()
{
}

Star SpatialIndex::getStar(const std::string& name) const
{
    auto it = names.find(name);
    if (it == names.end())
    {
        std::ostringstream os; 
        os << "Unknown star: " << name;
        throw std::invalid_argument(os.str());
    }

    return it->second;
}

NeighborMap SpatialIndex::getNeighborMap(double t, double tolerance) const
{
    auto it = maps.find(t);
    if (it != maps.end())
    {
        return it->second;
    }

    NeighborMap result(t);
    
    for (auto kv : names)
    {
        result.add(kv.second);
    }
    maps.emplace(t, result);
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
    return getNeighborMap(threshold, 10.0).nearestNeighbor(star, threshold);
}

Star 
SpatialIndex::nearestNeighbor(const std::string& name, double threshold) const
{
    return nearestNeighbor(getStar(name), threshold);
}

StarSet 
SpatialIndex::neighbors(const Star& star, double threshold) const
{
    return getNeighborMap(threshold).neighbors(star, threshold);
}

StarSet 
SpatialIndex::neighbors(const std::string& name, double threshold) const
{
    return neighbors(getStar(name), threshold);
}

StarList 
SpatialIndex::path(const Star& from, const Star& to, double threshold) const
{
    NeighborMap m = getNeighborMap(threshold);
    
    StarMap<Star> prev;
    PMap<Star> prev_prop(prev);
    StarMap<boost::default_color_type> colors;
    PMap<boost::default_color_type> color_prop(colors);

    auto p = boost::make_bfs_visitor(
        boost::record_predecessors(prev_prop, boost::on_tree_edge())
    );
    boost::breadth_first_search(m, from, boost::visitor(p).color_map(color_prop));

    StarList result;
    Star c = to;

    while (c != from)
    {
        result.push_back(c);
        auto n = get(prev_prop, c);
        if (n == Star()) break;
        c = n;
    }

    if (c != from)
    {
        return StarList();
    }

    result.push_back(from);
    std::reverse(result.begin(), result.end());

    return result;
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
    StarSet r;
    StarMap<boost::default_color_type> color_map;
    PMap<boost::default_color_type> colors(color_map);
    auto v = boost::make_bfs_visitor(
        lambda_visitor<boost::on_discover_vertex>(
            [&r](const Star& s, const NeighborMap& g)
            {
                r.insert(s);
            }
        )
    );

    boost::breadth_first_search(getNeighborMap(threshold),
        star,
        boost::visitor(v).
        color_map(colors)
    );

    return r;
}

StarSet 
SpatialIndex::reachable(const std::string& name, double threshold) const
{
    return reachable(getStar(name), threshold);
}

std::list<StarSet> 
SpatialIndex::connectedComponents(double threshold) const
{
    std::set<Star> q;
    std::transform(
        names.begin(), names.end(),
        inserter(q, q.end()),
        [](NameEntry kv) { return kv.second; }
    );

    std::list<StarSet> result;
    while (!q.empty())
    {
        auto v = *q.begin();
        q.erase(q.begin());

        auto r = reachable(v, threshold);
        for (auto u : r)
        {
            q.erase(u);
        }

        result.push_back(r);
    }

    return result;
}
