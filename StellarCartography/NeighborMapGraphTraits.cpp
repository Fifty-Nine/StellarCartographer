#include "StellarCartography/NeighborMapGraphTraits.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/range.hpp>
#include <boost/range/any_range.hpp>

using namespace StellarCartography;

namespace boost
{

typedef graph_traits<NeighborMap> traits;

auto traits::table(const graph_type& g)
    -> const graph_type::NeighborTable&
{
    return g.nodes_;
}

auto traits::vertices(const NeighborMap& g)
    -> vertex_iterator_pair
{
    auto& t = table(g);

    auto fcn = [](const vertex_transform_arg& a) -> Star
    { 
        return a.first;
    };

    return std::make_pair(
        vertex_iterator(t.begin(), fcn),
        vertex_iterator(t.end(), fcn)
    );
}

auto traits::num_vertices(const NeighborMap& g)
    -> vertices_size_type
{
    return table(g).size();
}

auto traits::source(
    const edge_descriptor& e, 
    const NeighborMap& g)
    -> vertex_descriptor
{
    return e.first;
}

auto traits::target(
    const edge_descriptor& e, 
    const NeighborMap& g)
    -> vertex_descriptor
{
    return e.second;
}

auto traits::out_edges(
    const vertex_descriptor& u,
    const NeighborMap& g)
    -> out_edge_iterator_pair 
{
    auto& t = table(g);
    auto it = t.find(u);
    auto& range = it->second;
    auto fcn = out_edge_transform_fcn(
        [u](const out_edge_transform_arg& v)
        {
            return edge_descriptor(u, v.second);
        }
    );
    
    auto begin = out_edge_iterator(
        range.begin(),
        fcn
    );
    auto end = out_edge_iterator(
        range.end(),
        fcn
    );
    
    return std::make_pair(begin, end);
}

auto traits::out_degree(
    const vertex_descriptor& v,
    const NeighborMap& g)
    -> degree_size_type
{
    return table(g).at(v).size();
}


} /* namespace boost */
