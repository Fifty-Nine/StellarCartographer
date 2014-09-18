#include "StellarCartography/StarMap.h"

#include <boost/concept_check.hpp>
#include <boost/graph/graph_concepts.hpp>

using namespace StellarCartography;
using namespace boost;

namespace
{

void concept_check [[gnu::unused]]() 
{
    std::vector<Star> v;
    StarMap(v.begin(), v.end());
    StarMap { };
    StarMap { Star(), Star() };
    StarMap s1;
    StarMap s2(s1);
    StarMap s3(std::move(s2));
    BOOST_CONCEPT_ASSERT((RandomAccessContainer<StarMap>));
    BOOST_CONCEPT_ASSERT((GraphConcept<StarMap>));
    BOOST_CONCEPT_ASSERT((VertexListGraphConcept<StarMap>));
    BOOST_CONCEPT_ASSERT((EdgeListGraphConcept<StarMap>));
    BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<StarMap>));
}

}

StarMap::StarMap() : 
    StarMap((Star*)nullptr, (Star*)nullptr)
{
}

StarMap::StarMap(const std::initializer_list<Star>& l) :
    StarMap(l.begin(), l.end())
{

}

Star StarMap::StarMap::getStar(const std::string& name) const
{
    auto it = byName().find(name);
    if (it == byName().end())
    {
        std::ostringstream os;
        os << "Unknown star: " << name;
        throw std::invalid_argument(os.str());
    }
    return *it;
}

Star StarMap::nearestNeighbor(const std::string& name, double threshold) const
{
    return Star();
}

Star StarMap::nearestNeighbor(const Star& star, double threshold) const
{
    return Star();
}

StarSet StarMap::neighbors(const std::string& name, double threshold) const
{
    return StarSet();
}

StarSet StarMap::neighbors(const Star& star, double threshold) const
{
    return StarSet();
}

StarList StarMap::path(
    const std::string& from, 
    const std::string& to, 
    double threshold) const
{
    return StarList();
}

StarList StarMap::path(
    const Star& from, 
    const Star& to, 
    double threshold) const
{
    return StarList();
}

StarSet StarMap::reachable(const std::string& name, double threshold) const
{
    return StarSet();
}

StarSet StarMap::reachable(const Star& star, double threshold) const
{
    return StarSet();
}

std::list<StarSet> StarMap::connectedComponents(double threshold) const
{
    return std::list<StarSet>();
}

std::pair<StarMap::vertex_iterator,StarMap::vertex_iterator>
StellarCartography::vertices(const StarMap& g)
{
    return std::make_pair(g.begin(), g.end());
}

StarMap::vertices_size_type
StellarCartography::num_vertices(const StarMap& g)
{
    return g.size();
}

Star StellarCartography::source(const Jump& j, const StarMap&)
{
    return j.source();
}

Star StellarCartography::target(const Jump& j, const StarMap&)
{
    return j.target();
}

std::pair<StarMap::out_edge_iterator, StarMap::out_edge_iterator>
StellarCartography::out_edges(const Star& u, const StarMap& g)
{
    StarMap::out_edge_filter_fcn p = 
    [u](const Star& v)
    {
        return u != v;
    };

    StarMap::out_edge_iterator_fcn t = 
    [u](const Star& v)
    {
        return Jump(u, v);
    };

    auto begin_filt = StarMap::out_edge_iterator_base(p, g.begin());
    auto end_filt = StarMap::out_edge_iterator_base(p, g.end());

    auto begin = StarMap::out_edge_iterator(begin_filt, t);
    auto end = StarMap::out_edge_iterator(end_filt, t);

    return std::make_pair(begin, end);
}

StarMap::degree_size_type
StellarCartography::out_degree(const Star& u, const StarMap& g)
{
    return g.size() - 1; 
}

std::pair<StarMap::edge_iterator, StarMap::edge_iterator>
StellarCartography::edges(const StarMap& g)
{
    return std::make_pair(
        StarMap::edge_iterator(g.begin(), g.end()),
        StarMap::edge_iterator(g.end(), g.end())
    );
}

StarMap::edges_size_type 
StellarCartography::num_edges(const StarMap& g)
{
    auto n = num_vertices(g); 
    return (n * (n - 1)) / 2;
}
