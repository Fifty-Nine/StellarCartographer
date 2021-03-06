#ifndef SC_STAR_MAP_H
#define SC_STAR_MAP_H

#include "StellarCartography/Jump.h"
#include "StellarCartography/Star.h"

#include <boost/container/flat_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/function_input_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <flann/flann.hpp>
#include <pairs_iterator.hpp>
#include <unordered_map>

namespace StellarCartography
{

using namespace boost;
using namespace boost::multi_index;

class StarMap
{
    typedef multi_index_container<
        Star,
        indexed_by<
            random_access<>,
            ordered_unique<identity<Star>>,
            hashed_unique<const_mem_fun<Star, std::string, &Star::getName>>,
            hashed_unique<const_mem_fun<Star, Coordinate, &Star::getCoords>>
        >
    > container_type;

    typedef flann::KDTreeSingleIndex<flann::L2<double>> spatial_type;
    typedef std::unique_ptr<spatial_type> spatial_ptr_type;
    typedef std::vector<double> spatial_storage_type;
    typedef flann::Matrix<double> matrix_type;

    enum { 
        SeqIndex,
        NameSeqIndex,
        NameMapIndex,
        CoordinateIndex
    };

    typedef container_type::nth_index<SeqIndex>::type seq_index;
    typedef container_type::nth_index<NameSeqIndex>::type name_seq_index;
    typedef container_type::nth_index<NameMapIndex>::type name_map_index;
    typedef container_type::nth_index<CoordinateIndex>::type coord_index;

public:
    /**************************************************************************/
    /* Constructors, destructors, assignment operators.                       */
    /**************************************************************************/
    ~StarMap() = default;
    StarMap();
    StarMap(const StarMap&);
    StarMap(StarMap&&);
    StarMap(const std::initializer_list<Star>&);
    template<class It>
    explicit StarMap(It begin, It end); 

    StarMap& operator=(const StarMap&);
    StarMap& operator=(StarMap&&);

    /**************************************************************************/
    /* RandomAccessContainer concept requirements                             */
    /**************************************************************************/
    typedef typename seq_index::value_type value_type;
    typedef typename seq_index::reference reference;
    typedef typename seq_index::const_reference const_reference;
    typedef typename seq_index::pointer pointer;
    typedef typename seq_index::const_pointer const_pointer;
    typedef typename seq_index::const_iterator iterator;
    typedef typename seq_index::const_iterator const_iterator;
    typedef typename seq_index::const_reverse_iterator reverse_iterator;
    typedef typename seq_index::const_reverse_iterator const_reverse_iterator;
    typedef typename seq_index::difference_type difference_type;
    typedef typename seq_index::size_type size_type;

    const_iterator begin() const { return byIndex().begin(); }
    const_iterator end() const { return byIndex().end(); }
    
    const Star& operator[](size_type n) const { return byIndex()[n]; }

    size_type size() const { return byIndex().size(); }
    size_type max_size() const { return byIndex().max_size(); }
    bool empty() const { return byIndex().empty(); }
        
    const_reverse_iterator rbegin() const { return byIndex().rbegin(); }
    const_reverse_iterator rend() const { return byIndex().rend(); }

    /**************************************************************************/
    /* Graph concept requirements.                                            */
    /**************************************************************************/
    typedef Star vertex_descriptor;
    typedef Jump edge_descriptor;
    typedef boost::undirected_tag directed_category;
    typedef boost::disallow_parallel_edge_tag edge_parallel_category;
    struct traversal_category : 
        public virtual boost::vertex_list_graph_tag,
        public virtual boost::edge_list_graph_tag,
        public virtual boost::incidence_graph_tag
    { };

    static Star null_vertex() { return Star(); }

    /**************************************************************************/
    /* VertexListGraph concept requirements.                                  */
    /**************************************************************************/
    typedef size_type vertices_size_type;
    typedef const_iterator vertex_iterator;
    
    /**************************************************************************/
    /* IncidenceGraph concept requirements.                                   */
    /**************************************************************************/
    typedef std::function<Jump(const Star&)> out_edge_iterator_fcn;
    typedef std::function<bool(const Star&)> out_edge_filter_fcn;
    typedef filter_iterator<out_edge_filter_fcn, vertex_iterator> 
        out_edge_iterator_base;
    typedef transform_iterator<out_edge_iterator_fcn, out_edge_iterator_base> 
        out_edge_iterator; 
    typedef size_type degree_size_type;

    /**************************************************************************/
    /* EdgeListGraph concept requirements.                                    */
    /**************************************************************************/
    typedef size_type edges_size_type;
    typedef pairs_iterator<vertex_iterator, Jump> edge_iterator;

    /**************************************************************************/
    /* PropertyGraph(Star, vertex_index_t) concept requirements.              */
    /**************************************************************************/
    typedef std::function<size_type(vertex_descriptor)> vertex_index_fcn;
    typedef function_property_map<
        vertex_index_fcn, 
        vertex_descriptor, 
        size_type
    > vertex_index_map;

    /**************************************************************************/
    /* Adaptor for the various graph concept requirements with edges filtered */
    /* by distance.                                                           */
    /**************************************************************************/
    class dist_index
    {
        double t2_;
        const StarMap *m_;

        typedef JumpSet edge_container;
        typedef std::unordered_map<Star, edge_container> edge_map;

    public:
        dist_index(double t2, const StarMap *m) :
            t2_(t2), m_(m)
        { 
            init(); 
        }

        const StarMap& parent() const { return *m_; }

        const edge_container& edges() const { return edges_; }
        const edge_container& neighbors(const Star& s) const;
 
        /* Graph concept */
        typedef StarMap::vertex_descriptor vertex_descriptor;
        typedef StarMap::edge_descriptor edge_descriptor;
        typedef StarMap::directed_category directed_category;
        typedef StarMap::edge_parallel_category edge_parallel_category;
        typedef StarMap::traversal_category traversal_category;
    
        static Star null_vertex() { return Star(); }

        /* VertexListGraph concept */
        typedef StarMap::vertices_size_type vertices_size_type;
        typedef StarMap::vertex_iterator vertex_iterator;

        /* EdgeListGraph concept */
        typedef edge_container::size_type edges_size_type;
        typedef edge_container::const_iterator edge_iterator;

        /* IncidenceGraph concept */
        typedef edge_map::value_type::second_type::const_iterator 
            out_edge_iterator;
        typedef edge_map::value_type::second_type::size_type degree_size_type;

        /* PropertyGraph(Star, vertex_index_t) concept */
        typedef StarMap::vertex_index_map vertex_index_map;

    private:
        edge_container edges_;
        edge_map neighbors_;

        void init();
    };

    /**************************************************************************/
    /* Container views.                                                       */
    /**************************************************************************/
    const seq_index& byIndex() const { return stars_.get<SeqIndex>(); }
    
    const name_seq_index& bySortedIndex() const 
    { return stars_.get<NameSeqIndex>(); }

    const name_map_index& byName() const { return stars_.get<NameMapIndex>(); }

    const coord_index& byCoordinate() const 
    { return stars_.get<CoordinateIndex>(); }

    const dist_index& byDistance(double threshold) const; 

    vertex_index_map vertexIndexMap() const; 

    /**************************************************************************/
    /* Algorithms                                                             */
    /**************************************************************************/
    Star getStar(const std::string& name) const;
    size_type getIndex(const Star& star) const;
    Star nearestNeighbor(const std::string& name, double threshold) const;
    Star nearestNeighbor(const Star& star, double threshold) const;

    StarSet neighbors(const std::string& name, double threshold) const;
    StarSet neighbors(const Star& star, double threshold) const;

    StarList path(
        const std::string& from, 
        const std::string& to, 
        double threshold) const;
    StarList path(
        const Star& from, 
        const Star& to, 
        double threshold) const;

    StarSet reachable(const std::string& name, double threshold) const;
    StarSet reachable(const Star& star, double threshold) const;

    std::vector<StarSet> connectedComponents(double threshold) const;
    Coordinate centerOfMass() const;
    double extent() const;

private:
    typedef container::flat_map<double, dist_index> dist_index_cache;

    template<class It>
    static spatial_storage_type initSpatialStorage(It begin, It end);
    spatial_ptr_type initIndex();

    container_type stars_;
    spatial_storage_type  spatial_storage_;
    mutable spatial_ptr_type spatial_index_;
    mutable dist_index_cache dist_index_cache_;
};

std::pair<StarMap::vertex_iterator,StarMap::vertex_iterator>
vertices(const StarMap& g);

std::pair<
    StarMap::dist_index::vertex_iterator,
    StarMap::dist_index::vertex_iterator
>
vertices(const StarMap::dist_index& g);

StarMap::vertices_size_type
num_vertices(const StarMap& g);

StarMap::dist_index::vertices_size_type
num_vertices(const StarMap::dist_index& g);

Star source(const Jump& j, const StarMap& g);
Star target(const Jump& j, const StarMap& g);
Star source(const Jump& j, const StarMap::dist_index& g);
Star target(const Jump& j, const StarMap::dist_index& g);

std::pair<StarMap::out_edge_iterator,StarMap::out_edge_iterator>
out_edges(const Star& u, const StarMap& g);

StarMap::degree_size_type
out_degree(const Star& u, const StarMap& g);

std::pair<
    StarMap::dist_index::out_edge_iterator,
    StarMap::dist_index::out_edge_iterator
>
out_edges(const Star& u, const StarMap::dist_index& g);

StarMap::dist_index::degree_size_type
out_degree(const Star& u, const StarMap::dist_index& g);

std::pair<StarMap::edge_iterator,StarMap::edge_iterator>
edges(const StarMap& g);

StarMap::edges_size_type
num_edges(const StarMap& g);

std::pair<
    StarMap::dist_index::edge_iterator,
    StarMap::dist_index::edge_iterator
>
edges(const StarMap::dist_index& g);

StarMap::dist_index::edges_size_type
num_edges(const StarMap::dist_index& g);

StarMap::vertex_index_map get(vertex_index_t, const StarMap& g);
StarMap::size_type get(
    vertex_index_t, const StarMap& g, const StarMap::vertex_descriptor& x);

StarMap::dist_index::vertex_index_map 
get(vertex_index_t, const StarMap::dist_index& g);

StarMap::dist_index::vertices_size_type get(
    vertex_index_t, 
    const StarMap::dist_index& g, 
    const StarMap::dist_index::vertex_descriptor& x);

template<class It>
auto StarMap::initSpatialStorage(It begin, It end)
    -> spatial_storage_type
{
    spatial_storage_type result;
    result.reserve(distance(begin, end) * 3);

    for (auto it = begin; it != end; ++it)
    {
        auto c = it->getCoords();
        result.push_back(c.x());
        result.push_back(c.y());
        result.push_back(c.z());
    }
    return result;
}

template<class It>
StarMap::StarMap(It begin, It end) : 
    stars_(begin, end), 
    spatial_storage_(
        std::move(initSpatialStorage(begin, end))
    ),
    spatial_index_(std::move(initIndex()))
{
}

} /* namespace StellarCartography */

namespace boost
{

template<>
struct property_map<StellarCartography::StarMap, vertex_index_t> 
{ 
    typedef StellarCartography::StarMap::vertex_index_map type;
    typedef StellarCartography::StarMap::vertex_index_map const_type;
};

template<>
struct property_map<StellarCartography::StarMap::dist_index, vertex_index_t> 
{ 
    typedef StellarCartography::StarMap::dist_index::vertex_index_map type;
    typedef StellarCartography::StarMap::dist_index::vertex_index_map const_type;
};

}

#endif /* SC_STAR_MAP_H */
