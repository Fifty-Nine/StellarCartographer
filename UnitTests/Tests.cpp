#include "UnitTests/Tests.h"

#include "StellarCartography/Coordinate.h"
#include "StellarCartography/Jump.h"
#include "StellarCartography/Star.h"

using namespace boost::test_tools;
using namespace StellarCartography;

std::ostream& 
StellarCartography::operator<<(std::ostream& os, const Coordinate& c)
{
    return os << "Coord(" << c.x() << "," << c.y() << "," << c.z() << ")";
}

std::ostream&
StellarCartography::operator<<(std::ostream& os, const Star& s)
{
    return os << "Star(" << s.getName() << "," << s.getCoords() << ")";
}

std::ostream&
StellarCartography::operator<<(std::ostream& os, const Jump& j)
{
    return os << "Jump(" 
              << j.source().getName() << "," 
              << j.target().getName() << ")";
}
