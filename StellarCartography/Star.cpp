#include "StellarCartography/Star.h"

#include <boost/concept_check.hpp>

using namespace StellarCartography;
using namespace boost;

namespace
{

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
    return compare(s) == 0;
}

bool Star::operator<(const Star& s) const
{
    return compare(s) < 0;
}

int Star::compare(const Star& o) const
{
    if (auto c = getName().compare(o.getName()))
    {
        return c;
    }

    return getCoords().compare(o.getCoords());
}

std::size_t std::hash<Star>::operator()(const Star& s) const
{
    return std::hash<std::string>()(s.getName());  
}
