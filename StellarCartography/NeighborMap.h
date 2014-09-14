#ifndef SC_NEIGHBOR_MAP
#define SC_NEIGHBOR_MAP

#include <list>
#include <map>
#include <string>

#include "StellarCartography/Star.h"

namespace StellarCartography
{

class NeighborMap
{
    double t2_;
    std::map<std::string, std::map<double, Star>> nodes_;

public:
    NeighborMap(double threshold);

    void add(const Star& s);
    void remove(const Star& s);
    std::list<Star> neighbors(const std::string& name, double threshold) const;
    Star nearestNeighbor(const std::string& name, double threshold) const;
};

} /* namespace StellarCartography */

#endif /* SC_NEIGHBOR_MAP */
