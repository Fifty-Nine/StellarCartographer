#ifndef SC_NEIGHBOR_MAP_GRAPH_TRAITS_H
#define SC_NEIGHBOR_MAP_GRAPH_TRAITS_H

#include <boost/graph/graph_traits.hpp>
#include <boost/range/any_range.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "StellarCartography/NeighborMap.h"
#include "StellarCartography/Star.h"

namespace StellarCartography
{

class NeighborMap;
class Star;

}

namespace boost 
{

template<>
struct graph_traits<StellarCartography::NeighborMap>
{
public:
    typedef StellarCartography::NeighborMap graph_type;

    /* Graph concept requirements */
    typedef StellarCartography::Star vertex_descriptor;
    typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;
    typedef boost::undirected_tag directed_category;
    typedef boost::disallow_parallel_edge_tag edge_parallel_category;
    struct traversal_category : 
        public virtual boost::vertex_list_graph_tag,
        public virtual boost::incidence_graph_tag
    { };

    static vertex_descriptor null_vertex() { return vertex_descriptor(); }

private:
    static const graph_type::NeighborTable& table(const graph_type& g);

public:

    /* VertexListGraph concept requirements */
    typedef graph_type::TableEntry vertex_transform_arg;
    typedef std::function<
        vertex_descriptor(
            const vertex_transform_arg& t
        )
    > vertex_transform_fcn;
    typedef boost::transform_iterator<
        vertex_transform_fcn,
        graph_type::NeighborTable::const_iterator
    > vertex_iterator;
    typedef std::size_t vertices_size_type;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_iterator_pair;

    static 
    vertex_iterator_pair
    vertices(const graph_type& g);

    static
    vertices_size_type
    num_vertices(const graph_type& g);

    /* IncidenceGraph concept requirements */
    typedef const graph_type::MapEntry& out_edge_transform_arg;
    typedef std::function<
        edge_descriptor(
            const out_edge_transform_arg&
        )
    > out_edge_transform_fcn;
    typedef boost::transform_iterator<
        out_edge_transform_fcn, 
        graph_type::DistanceMap::const_iterator
    > out_edge_iterator;
    typedef std::size_t degree_size_type;
    typedef 
        std::pair<out_edge_iterator, out_edge_iterator> 
        out_edge_iterator_pair;

    static
    vertex_descriptor source(
        const edge_descriptor& e,
        const graph_type& g
    );
    
    static
    vertex_descriptor target(
        const edge_descriptor& e,
        const graph_type& g
    );

    static
    out_edge_iterator_pair
    out_edges(
        const vertex_descriptor& u,
        const graph_type& g
    );

    static
    degree_size_type
    out_degree(
        const vertex_descriptor& v,
        const graph_type& g
    );
};

} /* namespace boost */

namespace StellarCartography
{

typedef boost::graph_traits<NeighborMap> NeighborMapTraits;

inline
NeighborMapTraits::vertex_iterator_pair
vertices(const NeighborMap& g)
{
    return NeighborMapTraits::vertices(g);
}

inline
NeighborMapTraits::vertices_size_type
num_vertices(const NeighborMap& g)
{
    return NeighborMapTraits::num_vertices(g);
}

/* IncidenceGraph concept requirements */
inline
NeighborMapTraits::vertex_descriptor 
source(
    const NeighborMapTraits::edge_descriptor& e, 
    const NeighborMap& g)
{
    return NeighborMapTraits::source(e, g);
}

inline
NeighborMapTraits::vertex_descriptor 
target(
    const NeighborMapTraits::edge_descriptor& e, 
    const NeighborMap& g)
{
    return NeighborMapTraits::target(e, g);
}

inline
NeighborMapTraits::out_edge_iterator_pair
out_edges(
    const NeighborMapTraits::vertex_descriptor& u,
    const NeighborMap& g)
{
    return NeighborMapTraits::out_edges(u, g);
}

inline
NeighborMapTraits::degree_size_type
out_degree(
    const NeighborMapTraits::vertex_descriptor& u,
    const NeighborMap& g)
{
    return NeighborMapTraits::out_degree(u, g);
}

} /* namespace StellarCartography */

#endif /* SC_NEIGHBOR_MAP_GRAPH_TRAITS_H */
