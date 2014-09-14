#include "UnitTests/Tests.h"

#include "StellarCartography/Coordinate.h"
#include "StellarCartography/Star.h"

using namespace boost::test_tools;
using namespace StellarCartography;

void 
print_log_value<Coordinate>::operator()(std::ostream& os, const Coordinate& c)
{
    os << "Coord(" << c.x() << "," << c.y() << "," << c.z() << ")";
}

void 
print_log_value<Star>::operator()(std::ostream& os, const Star& s)
{
    os << "Star(" << s.getName() << ",";
    print_log_value<Coordinate>()(os, s.getCoords());
    os << ")";
}

void 
print_log_value<StarList>::operator()(
    std::ostream& os, 
    const StarList& sl)
{
    os << "StarList(";
    for (auto s : sl)
    {
        print_log_value<Star>()(os, s);
        os << ",";
    }
    os << ")";
}

void 
print_log_value<StarSet>::operator()(
    std::ostream& os, 
    const StarSet& sl)
{
    os << "StarSet(";
    for (auto s : sl)
    {
        print_log_value<Star>()(os, s);
        os << ",";
    }
    os << ")";
}

