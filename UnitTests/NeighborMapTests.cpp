#include "UnitTests/Tests.h"

#include <cmath>
#include "StellarCartography/NeighborMap.h"

using namespace StellarCartography;

SC_TEST_SUITE(NeighborMapTests)

SC_TEST_CASE(NeighborMapTests, TestBasic)
{
    NeighborMap nm(10.0);
    Star a = { "A", {   0,   0,   0 } };
    Star b = { "B", {   1,   1,   1 } };
    Star c = { "C", {  -1,  -1,  -1 } };
    Star d = { "D", {  10,  10,  10 } };
    Star e = { "E", { -10, -10, -10 } };
    Star f = { "F", {  -9,  -9,  -9 } };
    Star g = { "G", { -10, -11,  -9 } };
    Star h = { "H", {   1,   0,   0 } };
    nm.add(a);
    nm.add(b);
    nm.add(c);
    nm.add(d);
    nm.add(e);
    nm.add(f);
    nm.add(g);
    nm.add(h);

    BOOST_CHECK_EQUAL(
        (StarSet { b, c }),
        nm.neighbors(a, 10.0)
    );

    BOOST_CHECK_THROW(nm.neighbors(a, 11.0), std::out_of_range);

    BOOST_CHECK_EQUAL(
        StarSet(),
        nm.neighbors(a, 1.0)
    );

    BOOST_CHECK_EQUAL(
        (StarSet { f, g }),
        nm.neighbors(e, 3.0)
    );

    BOOST_CHECK_EQUAL(h, nm.nearestNeighbor(a, 5.0));
    BOOST_CHECK_EQUAL(h, nm.nearestNeighbor(a, 10.0));
    BOOST_CHECK_EQUAL(Star(), nm.nearestNeighbor(a, 0.5));
    BOOST_CHECK_EQUAL(e, nm.nearestNeighbor(f, 10.0));

    BOOST_CHECK_THROW(nm.nearestNeighbor(Star(), 1.0), std::invalid_argument);
    BOOST_CHECK_THROW(nm.nearestNeighbor(a, -1.0), std::domain_error);
    BOOST_CHECK_THROW(
        nm.nearestNeighbor(a, std::numeric_limits<double>::quiet_NaN()), 
        std::domain_error
    );
    BOOST_CHECK_THROW(nm.nearestNeighbor(a, 11.0), std::out_of_range);
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
