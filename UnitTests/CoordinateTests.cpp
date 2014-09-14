#include "Tests.h"

#include <cmath>
#include "StellarCartography/Coordinate.h"

using namespace StellarCartography;

namespace boost {
namespace test_tools {

template<> 
struct print_log_value<Coordinate> 
{
    void operator()(std::ostream& os, const Coordinate& c)
    {
        os << "Coord(" << c.x() << "," << c.y() << "," << c.z() << ")";
    }
};

}
}

SC_TEST_SUITE(CoordinateTests)

SC_TEST_CASE(CoordinateTests, Sanity)
{
    Coordinate dfl;
    Coordinate origin(0.0, 0.0, 0.0);
    Coordinate xh(1.0, 0.0, 0.0);
    Coordinate yh(0.0, 1.0, 0.0);
    Coordinate zh(0.0, 0.0, 1.0);
    Coordinate xyz(1.0, 1.0, 1.0);

    BOOST_CHECK_EQUAL(dfl, origin);
    BOOST_CHECK_EQUAL(origin.distanceSquared(xh), 1.0);
    BOOST_CHECK_EQUAL(origin.distanceSquared(yh), 1.0);
    BOOST_CHECK_EQUAL(origin.distanceSquared(zh), 1.0);

    BOOST_CHECK_EQUAL(origin.distance(xyz), sqrt(3));
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
