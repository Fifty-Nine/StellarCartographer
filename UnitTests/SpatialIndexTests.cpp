#include "UnitTests/Tests.h"

#include <cmath>
#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

SC_TEST_SUITE(SpatialIndexTests)

Star sol() { 
    return { "Sol", { 0.0, 0.0, 0.0 } }; 
}

Star proximaCentauri() {
    return { "Proxima Centauri", { 4.0, 0.0, 0.0 } };
}

Star alphaCentauri() {
    return { "Alpha Centauri", { 6.0, 6.0, 6.0 } };
}

Star polaris() { 
    return { "Polaris", { -5.0, 0.0, 0.0 } };
}

Star sirius() { 
    return { "Sirius", { 4.96, -4.96, 4.96 } };
}

Star betaCanisMajoris() {
    return { "Beta Canis Majoris", { 10.0, 10.0, 10.0 } };
}

SpatialIndex basicGalaxy()
{
    SpatialIndex result;
    
    result.insert(sol());
    result.insert(proximaCentauri());
    result.insert(alphaCentauri());
    result.insert(polaris());
    result.insert(sirius());
    result.insert(betaCanisMajoris());

    return result;
}

SC_TEST_CASE(SpatialIndexTests, TestNearestNeighbor)
{
    SpatialIndex g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        proximaCentauri(), g.nearestNeighbor(sol().getName(), 5.0));
    BOOST_CHECK_EQUAL(
        betaCanisMajoris(), g.nearestNeighbor(alphaCentauri().getName(), 6.0)
    );
    BOOST_CHECK_EQUAL(
        Star(), g.nearestNeighbor(sirius(), 1.0)
    );
}

SC_TEST_CASE(SpatialIndexTests, TestNeighbors)
{
    SpatialIndex g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (StarSet { polaris(), proximaCentauri() }),
        (g.neighbors(sol(), 6.0))
    );

    BOOST_CHECK_EQUAL(
        StarSet(),
        (g.neighbors(sol().getName(), 1.0))
    );

    BOOST_CHECK_EQUAL(
        (StarSet { alphaCentauri() }),
        (g.neighbors(betaCanisMajoris(), 7))
    );
}

SC_TEST_CASE(SpatialIndexTests, TestPath)
{
    SpatialIndex g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (StarList { sol(), proximaCentauri() }),
        (g.path(sol(), proximaCentauri(), 5))
    );

    BOOST_CHECK_EQUAL(
        (StarList { sol(), alphaCentauri(), betaCanisMajoris() }),
        (g.path(sol().getName(), betaCanisMajoris().getName(), 10))
    );
}

SC_TEST_CASE(SpatialIndexTests, TestRoutingWeight)
{
    /* 
     * Verify we take the path with the fewest jumps, not least
     * total distance.
     */
    SpatialIndex g;
    Star a { "A", { 0, 0, 0 } };
    Star b { "B", { 1, 0, 0 } };
    Star c { "C", { 2, 0, 0 } };
    Star d { "D", { 3, 0, 0 } };
    Star e { "E", { 2, 1, 0 } };
    g.insert(a);
    g.insert(b);
    g.insert(c);
    g.insert(d);
    g.insert(e);

    
    BOOST_CHECK_EQUAL(
        (StarList { a, e, d }),
        (g.path(a, d, 2.5))
    );

    BOOST_CHECK_EQUAL(
        (StarList { a, d }),
        (g.path(a, d, 4.0))
    );
    
    BOOST_CHECK_EQUAL(
        (StarList { a, b, c, d }),
        (g.path(a, d, 1.1))
    );
}
SC_TEST_CASE_END()

SC_TEST_CASE(SpatialIndexTests, TestReachable)
{
    SpatialIndex g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (StarSet { sol(), proximaCentauri(), polaris() }),
        g.reachable(sol(), 5.1)
    );

    BOOST_CHECK_EQUAL(
        (StarSet { alphaCentauri(), betaCanisMajoris() }),
        (g.reachable(alphaCentauri().getName(), 7.0))
    );

    BOOST_CHECK_EQUAL(
        (StarSet { alphaCentauri(), betaCanisMajoris() }),
        (g.reachable(betaCanisMajoris(), 7.0))
    );
}
SC_TEST_CASE_END()

SC_TEST_CASE(SpatialIndexTests, TestConnectedComponents)
{
    SpatialIndex g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (
            std::list<StarSet> 
            {
                { sol(), proximaCentauri(), polaris() },
                { alphaCentauri(), betaCanisMajoris() },
                { sirius() }
            }
        ),
        g.connectedComponents(7.0)
    );
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
