#include "StellarCartography/SpatialIndex.h"

#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <limits>
#include <sstream>

using namespace StellarCartography;

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
    typedef std::pair<int, Star> PQEntry;
    typedef 
        boost::heap::fibonacci_heap<
            PQEntry, 
            boost::heap::compare<std::greater<PQEntry>>
        > PQ;
    typedef PQ::handle_type PQHandle;
        
    std::unordered_map<Star, int> dist;
    std::unordered_map<Star, Star> prev;
    std::unordered_map<Star, PQHandle> handles;

    PQ pq;

    for (auto kv : names)
    {
        auto v = kv.second;
        int d = (v == from) ? 0 : std::numeric_limits<int>::max();
        dist[v] = d;
        handles[v] = pq.push( PQEntry { d, v } );
    }

    while (!pq.empty())
    {
        auto u = pq.top().second;
        pq.pop();

        for (auto v : neighbors(u, threshold))
        {
            auto d = dist[u] + 1;
            if (d < dist[v])
            {
                dist[v] = d;
                prev[v] = u;
                pq.update(handles[v], PQEntry { d, v });
            }
        }
    }

    StarList result;
    Star c = to;
    decltype(prev)::const_iterator it;

    while ((it = prev.find(c)) != prev.end())
    {
        result.push_back(c);
        c = it->second;
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
    std::vector<Star> s;
    StarSet r;
    s.push_back(star);

    while (!s.empty())
    {
        auto v = s.back();
        s.pop_back();

        r.insert(v);

        for (auto u : neighbors(v, threshold))
        {
            if (r.find(u) == r.end())
            {
                s.push_back(u);
            }
        }
    }

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
