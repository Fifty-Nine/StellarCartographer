#ifndef SC_JUMP_H
#define SC_JUMP_H

#include <boost/operators.hpp>

#include "StellarCartography/Star.h"

namespace StellarCartography
{

class Jump : 
    boost::totally_ordered<Jump>
{
    std::pair<Star, Star> endpoints_;
public:
    Jump() = default;
    Jump(const Star& from, const Star& to);

    operator std::pair<Star, Star>& ();
    operator const std::pair<Star, Star>& () const;

    Star source() const;
    Star target() const;
    double weight() const;

    bool operator==(const Jump& j) const;
    bool operator<(const Jump& j) const;

    int compare(const Jump& j) const;
};

typedef std::list<Jump> JumpList;

} /* namespace StellarCartopgraphy */

#endif /* SC_JUMP_H */
