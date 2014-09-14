#ifndef SC_STAR_H
#define SC_STAR_H

#include <boost/operators.hpp>
#include <list>
#include <set>
#include <string>

#include "StellarCartography/Coordinate.h"

namespace StellarCartography
{

class Star : 
    boost::totally_ordered<Star>
{
    std::string name_;
    Coordinate coords_;
public:
    Star() { }
    Star(const std::string& name, const Coordinate& coords) :
        name_(name), coords_(coords)
    {
    }

    std::string getName() const { return name_; }
    Coordinate getCoords() const { return coords_; }

    bool operator==(const Star& s) const;
    bool operator<(const Star& s) const;
};

typedef std::list<Star> StarList;
typedef std::set<Star> StarSet;

}

#endif /* SC_STAR_H */
