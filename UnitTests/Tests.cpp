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

void print_log_value<std::list<Star>>::operator()(
    std::ostream& os, 
    const std::list<StellarCartography::Star>& sl)
{
    os << "StarList(";
    for (auto s : sl)
    {
        print_log_value<Star>()(os, s);
        os << ",";
    }
    os << ")";
}

