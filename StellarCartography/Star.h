#ifndef SC_STAR_H
#define SC_STAR_H

#include <string>

namespace StellarCartography
{

class Coordinate;

class Star
{
    std::string name_;
    Coordinate coords_;
public:
    Star(const std::string& name, const Coordinate& coords) :
        name_(name), coords_(coords)
    {
    }

    std::string getName() const { return name_; }
    Coordinate getCoords() const { return coords_; }
};

}

#endif /* SC_STAR_H */
