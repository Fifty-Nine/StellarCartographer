#include "StellarCartography/Star.h"

#include <boost/concept_check.hpp>

using namespace StellarCartography;
using namespace boost;

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

void concept_check()
{
    BOOST_CONCEPT_ASSERT((Assignable<Star>));
    BOOST_CONCEPT_ASSERT((DefaultConstructible<Star>));
    BOOST_CONCEPT_ASSERT((CopyConstructible<Star>));
    BOOST_CONCEPT_ASSERT((EqualityComparable<Star>));
    BOOST_CONCEPT_ASSERT((LessThanComparable<Star>));

    Star { "", Coordinate() };
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
