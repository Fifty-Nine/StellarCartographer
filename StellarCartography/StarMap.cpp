#include "StellarCartography/StarMap.h"

#include <boost/concept_check.hpp>

namespace StellarCartography
{

namespace
{

void concept_check()
{
    std::vector<Star> v;
    StarMap(v.begin(), v.end());
    StarMap { };
    StarMap { Star(), Star() };
    BOOST_CONCEPT_ASSERT((RandomAccessContainer<StarMap>));
}

}

StarMap::StarMap(const std::initializer_list<Star>& l) :
    stars_(l.begin(), l.end())
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

} /* namespace StellarCartography */
