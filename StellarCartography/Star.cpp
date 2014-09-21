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

std::string Star::getProperty(const std::string& key) const
{
    auto it = props_.find(key);
    if (it == props_.end()) 
    {
        std::ostringstream ss;
        ss << "No such property: " << key;
        throw std::invalid_argument(ss.str());
    }
    return it->second;
}

void Star::setProperty(const std::string& key, const std::string& value)
{
    props_[key] = value;
}

std::size_t std::hash<Star>::operator()(const Star& s) const
{
    return std::hash<std::string>()(s.getName());  
}
