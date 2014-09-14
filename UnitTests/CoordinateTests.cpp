#include "Tests.h"

#include <cmath>
#include "StellarCartography/Coordinate.h"

using namespace StellarCartography;

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

SC_TEST_CASE(CoordinateTests, InitList)
{
    Coordinate a(1.0, 2.0, 3.0);
    Coordinate b { 1.0, 2.0, 3.0 };

    BOOST_CHECK_EQUAL(a, b);
}

SC_TEST_SUITE_END()
