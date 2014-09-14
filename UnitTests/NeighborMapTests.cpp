#include "UnitTests/Tests.h"

#include <cmath>
#include "StellarCartography/NeighborMap.h"

using namespace StellarCartography;

SC_TEST_SUITE(NeighborMapTests)

bool contains(const std::string& name, const StarSet& l)
{
    return std::find_if(
        l.begin(), l.end(),
        [name](const Star& s)
        {
            return s.getName() == name;
        }
    ) != l.end();
}

SC_TEST_CASE(NeighborMapTests, TestBasic)
{
    NeighborMap nm(10.0);
    nm.add({ "A", {   0,   0,   0 } });
    nm.add({ "B", {   1,   1,   1 } });
    nm.add({ "C", {  -1,  -1,  -1 } });
    nm.add({ "D", {  10,  10,  10 } });
    nm.add({ "E", { -10, -10, -10 } });
    nm.add({ "F", {  -9,  -9,  -9 } });
    nm.add({ "G", { -10, -11,  -9 } });
    nm.add({ "H", {   1,   0,   0 } });

    auto neighbors = nm.neighbors("A", 10.0);
    BOOST_CHECK_EQUAL(2, neighbors.size());
    BOOST_CHECK(contains("B", neighbors));
    BOOST_CHECK(contains("C", neighbors));

    BOOST_CHECK_THROW(nm.neighbors("A", 11.0), std::out_of_range);

    neighbors = nm.neighbors("A", 1.0);
    BOOST_CHECK_EQUAL(StarSet(), neighbors);

    neighbors = nm.neighbors("E", 3.0);
    BOOST_CHECK_EQUAL(2, neighbors.size());
    BOOST_CHECK(contains("F", neighbors));
    BOOST_CHECK(contains("G", neighbors));

    BOOST_CHECK_EQUAL("H", nm.nearestNeighbor("A", 5.0).getName());
    BOOST_CHECK_EQUAL("H", nm.nearestNeighbor("A", 10.0).getName());
    BOOST_CHECK_EQUAL(Star(), nm.nearestNeighbor("A", 0.5));
    BOOST_CHECK_EQUAL("G", nm.nearestNeighbor("F", 10.0).getName());
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
