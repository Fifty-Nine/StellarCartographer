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

    BOOST_CONCEPT_ASSERT((GraphConcept<StarMap::dist_index>));
    BOOST_CONCEPT_ASSERT((VertexListGraphConcept<StarMap::dist_index>));
    BOOST_CONCEPT_ASSERT((EdgeListGraphConcept<StarMap::dist_index>));
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
    spatial_index_(initIndex())
{
}

StarMap::StarMap(StarMap&& m) : 
    stars_(std::move(m.stars_)),
    spatial_storage_(std::move(m.spatial_storage_)),
    spatial_index_(initIndex())
{
}

StarMap::StarMap(const std::initializer_list<Star>& l) :
    StarMap(l.begin(), l.end())
{

}

StarMap& StarMap::operator=(const StarMap& m) 
{
    *this = std::move(StarMap(m));
    return *this;
}

StarMap& StarMap::operator=(StarMap&& m)
{
    stars_ = std::move(m.stars_);
    spatial_storage_ = std::move(m.spatial_storage_);
    spatial_index_ = initIndex();

    return *this;
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
    return { 0, 0 };
}

auto StellarCartography::num_edges(const StarMap::dist_index& g)
    -> decltype(num_edges(g))
{
    return 0;
}
