#include <StellarCartography/Jump.h>

#include <boost/concept_check.hpp>

using namespace StellarCartography;
using namespace boost;

namespace
{

void concept_check()
{
    BOOST_CONCEPT_ASSERT((Assignable<Jump>));
    BOOST_CONCEPT_ASSERT((DefaultConstructible<Jump>));
    BOOST_CONCEPT_ASSERT((CopyConstructible<Jump>));
    BOOST_CONCEPT_ASSERT((EqualityComparable<Jump>));
    BOOST_CONCEPT_ASSERT((LessThanComparable<Jump>));
    BOOST_CONCEPT_ASSERT((Convertible<Jump, std::pair<Star, Star>>));

    Jump { Star(), Star() };
}

}

Jump::Jump(const Star& from, const Star& to) : 
    endpoints_(from, to)
{
}

Jump::operator std::pair<Star, Star>& ()
{
    return endpoints_;
}

Jump::operator const std::pair<Star, Star>& () const
{
    return endpoints_;
}

Star Jump::source() const
{
    return endpoints_.first;
}

Star Jump::target() const
{
    return endpoints_.second;
}

double Jump::weight() const
{
    return source().getCoords().distance(target().getCoords());
}

bool Jump::operator==(const Jump& j) const 
{
    return compare(j) == 0;
}

bool Jump::operator<(const Jump& j) const
{
    return compare(j) < 0;
}

int Jump::compare(const Jump& o) const
{
    if (auto c = source().compare(o.source())) return c;
    return target().compare(o.target());
}

std::size_t std::hash<Jump>::operator()(const Jump& s) const
{
    return std::hash<Star>()(s.source()) | 
        (std::hash<Star>()(s.target()) < 1);
}
