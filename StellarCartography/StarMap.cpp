#include "StellarCartography/StarMap.h"

#include <boost/concept_check.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/property_map/property_map.hpp>

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

    BOOST_CONCEPT_ASSERT((GraphConcept<StarMap::dist_index>));
    BOOST_CONCEPT_ASSERT((VertexListGraphConcept<StarMap::dist_index>));
    BOOST_CONCEPT_ASSERT((EdgeListGraphConcept<StarMap::dist_index>));
    BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<StarMap::dist_index>));

    BOOST_CONCEPT_ASSERT((
        ReadablePropertyMapConcept<
            StarMap::vertex_index_map, 
            StarMap::vertex_descriptor
        >
    ));

    BOOST_CONCEPT_ASSERT((
        ReadablePropertyGraphConcept<
            StarMap, 
            StarMap::vertex_descriptor,
            vertex_index_t
        >
    ));
    BOOST_CONCEPT_ASSERT((
        ReadablePropertyGraphConcept<
            StarMap::dist_index, 
            StarMap::dist_index::vertex_descriptor,
            vertex_index_t
        >
    ));
}

template<class T>
flann::Matrix<T> toMatrix(const T *value, int rows = 1, int dims = 1)
{
    return flann::Matrix<T>(const_cast<T*>(value), rows, dims);
}

flann::Matrix<double> toMatrix(const Coordinate *c)
{
    return flann::Matrix<double>(const_cast<double*>(c->data()), 1, 3);
}

}

StarMap::StarMap() : 
    StarMap((Star*)nullptr, (Star*)nullptr)
{
}

StarMap::StarMap(const StarMap& m) : 
    stars_(m.stars_),
    spatial_storage_(m.spatial_storage_),
    spatial_index_(initIndex()),
    dist_index_cache_(m.dist_index_cache_)
{
}

StarMap::StarMap(StarMap&& m) : 
    stars_(std::move(m.stars_)),
    spatial_storage_(std::move(m.spatial_storage_)),
    spatial_index_(initIndex()),
    dist_index_cache_(std::move(m.dist_index_cache_))
{
}

StarMap::StarMap(const std::initializer_list<Star>& l) :
    StarMap(l.begin(), l.end())
{

}

StarMap& StarMap::operator=(const StarMap& m) 
{
    if (&m != this)
        *this = std::move(StarMap(m));
    return *this;
}

StarMap& StarMap::operator=(StarMap&& m)
{
    stars_ = std::move(m.stars_);
    spatial_storage_ = std::move(m.spatial_storage_);
    spatial_index_ = initIndex();
    dist_index_cache_ = std::move(m.dist_index_cache_);

    return *this;
}

void StarMap::dist_index::init()
{
    std::vector<double> c(m_->size() * 3);

    auto p = c.data();
    for (auto v : *m_)
    {
        neighbors_[v] = JumpSet();
        Coordinate c(v.getCoords());
        std::copy(c.data(), c.data() + 3, p);
        p += 3;
    }

    std::vector<std::vector<int>> idx;
    std::vector<std::vector<double>> dists;

    m_->spatial_index_->radiusSearch(
        matrix_type(c.data(), m_->size(), 3),
        idx,
        dists,
        t2_,
        flann::SearchParams()
    );

    for (size_t from = 0; from < idx.size(); ++from)
    {
        auto& r = idx[from];
        for (size_t j = 0; j < r.size(); ++j)
        {
            size_t to = r[j]; 

            if (to > from) edges_.emplace((*m_)[from], (*m_)[to]);
        }
    }

    for (auto j : edges_)
    {
        neighbors_[j.source()].emplace(j.source(), j.target());
        neighbors_[j.target()].emplace(j.target(), j.source());
    }
}

auto StarMap::dist_index::neighbors(const Star& s) const
    -> decltype(neighbors(s))
{
    auto it = neighbors_.find(s);
    if (it == neighbors_.end())
    {
        std::ostringstream ss;
        ss << "Star " << s.getName() << " not in graph.";
        throw std::invalid_argument(ss.str());
    }

    return it->second;
}

auto StarMap::byDistance(double d) const
    -> decltype(byDistance(0.0))
{
    auto t2 = d*d;
    auto it = dist_index_cache_.find(t2);

    return (it != dist_index_cache_.end()) ? 
        it->second : 
        dist_index_cache_.emplace_hint(it, t2, dist_index(t2, this))->second;
}

auto StarMap::vertexIndexMap() const
    -> vertex_index_map
{
    return { bind(&StarMap::getIndex, this, _1) };
}

Star StarMap::getStar(const std::string& name) const
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

auto StarMap::getIndex(const Star& v) const
    -> size_type
{
    auto it = bySortedIndex().find(v);
    return stars_.project<SeqIndex>(it) - byIndex().begin();
}

Star StarMap::nearestNeighbor(const std::string& name, double threshold) const
{
    return nearestNeighbor(getStar(name), threshold);
}

Star StarMap::nearestNeighbor(const Star& star, double threshold) const
{
    auto c = star.getCoords();
    int index[2] = { 0 };
    double dist[2] = { 0 };
    flann::Matrix<int> i = toMatrix(index, 1, 2);
    flann::Matrix<double> d = toMatrix(dist, 1, 2);

    spatial_index_->knnSearch(toMatrix(&c), i, d, 2, flann::SearchParams());

    return (dist[1] < threshold*threshold) ? byIndex().at(index[1]) : Star();
}

StarSet StarMap::neighbors(const std::string& name, double threshold) const
{
    return neighbors(getStar(name), threshold);
}

StarSet StarMap::neighbors(const Star& star, double threshold) const
{
    auto c = star.getCoords();
    std::vector<std::vector<int>> idx;
    std::vector<std::vector<double>> dists;
    double t2 = threshold * threshold;

    spatial_index_->radiusSearch(
        toMatrix(&c),
        idx,
        dists,
        t2,
        flann::SearchParams()
    );

    auto pred = [star](const Star& v)
    {
        return v != star;
    };
    auto xform = [this](int i)
    {
        return byIndex().at(i);
    };
    
    /* lol, iterators */
    auto xbegin = make_transform_iterator(idx.front().begin(), xform);
    auto xend = make_transform_iterator(idx.front().end(), xform);

    auto begin = make_filter_iterator(pred, xbegin, xend);
    auto end = make_filter_iterator(pred, xend, xend);

    return StarSet(begin, end);
}

StarList StarMap::path(
    const std::string& from, 
    const std::string& to, 
    double threshold) const
{
    return path(getStar(from), getStar(to), threshold);
}

StarList StarMap::path(
    const Star& from, 
    const Star& to, 
    double threshold) const
{
    vector_property_map<Star, vertex_index_map> prev_pa(vertexIndexMap());

    breadth_first_search(
        byDistance(threshold),
        from,
        visitor(
            make_bfs_visitor(
                record_predecessors(prev_pa, on_tree_edge())
            )
        )
    );

    Star s = to;
    StarList result;

    while (s != from)
    {
        result.push_front(s);
        s = prev_pa[s];
    }

    if (s == from) result.push_front(s);

    return result;
}

StarSet StarMap::reachable(const std::string& name, double threshold) const
{
    return reachable(getStar(name), threshold);
}

StarSet StarMap::reachable(const Star& star, double threshold) const
{
    StarSet result;
    breadth_first_search(
        byDistance(threshold),
        star,
        visitor(
            make_bfs_visitor(
                write_property(
                    typed_identity_property_map<Star>(),
                    inserter(result, result.begin()), 
                    on_discover_vertex()
                )
            )
        )
    );

    return result; 
}

std::vector<StarSet> StarMap::connectedComponents(double threshold) const
{ 
    typedef std::vector<size_type> idx_vector;
    idx_vector c(size());
    iterator_property_map<idx_vector::iterator, vertex_index_map> 
        pa(c.begin(), vertexIndexMap());

    auto count = connected_components(byDistance(threshold), pa);

    std::vector<StarSet> result(count);
    for (size_type i = 0; i < c.size(); ++i)
    {
        Star s = byIndex().at(i);
        size_type component = c[i];

        result[component].insert(s);
    }

    return result;
}
        
auto StarMap::initIndex() 
    -> spatial_ptr_type
{
    /* No make_unique in C++11 :( */
    spatial_ptr_type result(
        new spatial_type(
            matrix_type(spatial_storage_.data(), stars_.size(), 3),
            flann::KDTreeSingleIndexParams(10, true)
        )
    );

    if (stars_.size() > 0) result->buildIndex();

    return result;
}

std::pair<StarMap::vertex_iterator,StarMap::vertex_iterator>
StellarCartography::vertices(const StarMap& g)
{
    return std::make_pair(g.begin(), g.end());
}

auto StellarCartography::vertices(const StarMap::dist_index& g)
    -> decltype(vertices(g))
{
    return vertices(g.parent());
}

StarMap::vertices_size_type
StellarCartography::num_vertices(const StarMap& g)
{
    return g.size();
}


auto StellarCartography::num_vertices(const StarMap::dist_index& g)
    -> decltype(num_vertices(g))
{
    return num_vertices(g.parent());
}

Star StellarCartography::source(const Jump& j, const StarMap&)
{
    return j.source();
}

Star StellarCartography::source(const Jump& j, const StarMap::dist_index&)
{
    return j.source();
}

Star StellarCartography::target(const Jump& j, const StarMap&)
{
    return j.target();
}

Star StellarCartography::target(const Jump& j, const StarMap::dist_index&)
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

auto StellarCartography::out_edges(const Star& u, const StarMap::dist_index& g)
    -> decltype(out_edges(u, g ))
{
    auto& n = g.neighbors(u);
    return { n.begin(), n.end() };
}

auto StellarCartography::out_degree(const Star& u, const StarMap::dist_index& g)
    -> decltype(out_degree(u, g))
{
    return g.neighbors(u).size();
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

auto StellarCartography::edges(const StarMap::dist_index& g)
    -> decltype(edges(g))
{
    return { g.edges().begin(), g.edges().end() };
}

auto StellarCartography::num_edges(const StarMap::dist_index& g)
    -> decltype(num_edges(g))
{
    return g.edges().size();
}

auto StellarCartography::get(vertex_index_t, const StarMap& g)
    -> decltype(get(vertex_index_t(), g))
{
    return g.vertexIndexMap();
}

auto StellarCartography::get(
    vertex_index_t p, const StarMap& g, const StarMap::vertex_descriptor& v)
    -> StarMap::size_type
{
    return get(get(p, g), v);
}

auto StellarCartography::get(
    vertex_index_t p, const StarMap::dist_index& g)
    -> decltype(get(p, g))
{
    return get(p, g.parent());
}

auto StellarCartography::get(
    vertex_index_t p, 
    const StarMap::dist_index& g, 
    const StarMap::dist_index::vertex_descriptor& v)
    -> decltype(get(p, g, v))
{
    return get(get(p, g), v);
}
