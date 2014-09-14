#include "StellarCartography/Star.h"

using namespace StellarCartography;

namespace
{

int compare(const Star& l, const Star& r)
{
    if (auto c = l.getName().compare(r.getName()))
    {
        return c;
    }

    return l.getCoords().compare(r.getCoords());
                                          
}

} /* namespace */

bool Star::operator==(const Star& s) const
{ 
    return compare(*this, s) == 0;
}

bool Star::operator<(const Star& s) const
{
    return compare(*this, s) < 0;
}

std::size_t std::hash<Star>::operator()(const Star& s) const
{
    return std::hash<std::string>()(s.getName());  
}
