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

void checkComparison(const Coordinate& l, const Coordinate& r, int comp)
{
    BOOST_CHECK_EQUAL((comp < 0), (l < r));
    BOOST_CHECK_EQUAL((comp <= 0), (l <= r));
    BOOST_CHECK_EQUAL((comp > 0), (l > r));
    BOOST_CHECK_EQUAL((comp >= 0), (l >= r));
    BOOST_CHECK_EQUAL((comp == 0), (l == r));
    BOOST_CHECK_EQUAL((comp != 0), (l != r));

    BOOST_CHECK_EQUAL((0 < comp), (r < l));
    BOOST_CHECK_EQUAL((0 <= comp), (r <= l));
    BOOST_CHECK_EQUAL((0 > comp), (r > l));
    BOOST_CHECK_EQUAL((0 >= comp), (r >= l));
    BOOST_CHECK_EQUAL((0 == comp), (r == l));
    BOOST_CHECK_EQUAL((0 != comp), (r != l));
}

SC_TEST_CASE(CoordinateTests, TestOrder)
{
    Coordinate origin;
    Coordinate a { 0, 0, 0 };
    Coordinate b { 1, 0, 0 };
    Coordinate c { 0, 1, 0 };
    Coordinate d { -1, -1, -1 };

    checkComparison(origin, a, 0);
    checkComparison(origin, b, -1);
    checkComparison(origin, c, -1);
    checkComparison(origin, d, 1);

    checkComparison(a, b, -1);
    checkComparison(a, c, -1);
    checkComparison(a, d, 1);

    checkComparison(b, c, 1);
    checkComparison(b, d, 1);

    checkComparison(c, d, 1);
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
