#ifndef SC_STAR_MAP_H
#define SC_STAR_MAP_H

#include "StellarCartography/Star.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

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
            hashed_unique<const_mem_fun<Star, std::string, &Star::getName>>
        >
    > container_type;

    enum { 
        SeqIndex = 0,
        NameSeqIndex = 1,
        NameMapIndex = 2
    };

    typedef container_type::nth_index<SeqIndex>::type seq_index;
    typedef container_type::nth_index<NameSeqIndex>::type name_seq_index;
    typedef container_type::nth_index<NameMapIndex>::type name_map_index;

public:
    /**************************************************************************/
    /* Constructors, destructors, assignment operators.                       */
    /**************************************************************************/
    ~StarMap() = default;
    StarMap() = default;
    StarMap(const StarMap&) = default;
    StarMap(StarMap&&) = default;
    StarMap(const std::initializer_list<Star>&);
    template<class It>
    explicit StarMap(It begin, It end); 

    StarMap& operator=(const StarMap&) = default;
    StarMap& operator=(StarMap&&) = default;

    /**************************************************************************/
    /* RandomAccessContainer requirements                                     */
    /**************************************************************************/
    typedef typename seq_index::value_type value_type;
    typedef typename seq_index::reference reference;
    typedef typename seq_index::const_reference const_reference;
    typedef typename seq_index::pointer pointer;
    typedef typename seq_index::const_pointer const_pointer;
    typedef typename seq_index::iterator iterator;
    typedef typename seq_index::const_iterator const_iterator;
    typedef typename seq_index::reverse_iterator reverse_iterator;
    typedef typename seq_index::const_reverse_iterator const_reverse_iterator;
    typedef typename seq_index::difference_type difference_type;
    typedef typename seq_index::size_type size_type;

    iterator begin() { return byIndex().begin(); }
    const_iterator begin() const { return byIndex().begin(); }

    iterator end() { return byIndex().end(); }
    const_iterator end() const { return byIndex().end(); }
    
    const Star& operator[](size_type n) const { return byIndex()[n]; }

    size_type size() const { return byIndex().size(); }
    size_type max_size() const { return byIndex().max_size(); }
    bool empty() const { return byIndex().empty(); }
        
    reverse_iterator rbegin() { return byIndex().rbegin(); }
    const_reverse_iterator rbegin() const { return byIndex().rbegin(); }

    reverse_iterator rend() { return byIndex().rend(); }
    const_reverse_iterator rend() const { return byIndex().rend(); }

    /**************************************************************************/
    /* Container views.                                                       */
    /**************************************************************************/
    typedef seq_index ByIndex;
    typedef name_seq_index BySortedIndex;
    typedef name_map_index ByName;

    ByIndex& byIndex() { return stars_.get<SeqIndex>(); }
    const ByIndex& byIndex() const { return stars_.get<SeqIndex>(); }
    
    BySortedIndex& bySortedIndex() 
    { return stars_.get<NameSeqIndex>(); }
    const BySortedIndex& bySortedIndex() const 
    { return stars_.get<NameSeqIndex>(); }

    ByName& byName() { return stars_.get<NameMapIndex>(); }
    const ByName& byName() const { return stars_.get<NameMapIndex>(); }

    /**************************************************************************/
    /* Algorithms                                                             */
    /**************************************************************************/
    Star getStar(const std::string& name) const;
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

    std::list<StarSet> connectedComponents(double threshold) const;

private:
    container_type stars_;
};

template<class It>
StarMap::StarMap(It begin, It end) : 
    stars_(begin, end)
{
}

} /* namespace StellarCartography */

#endif /* SC_STAR_MAP_H */
