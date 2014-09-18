#include "UnitTests/Tests.h"

#include <cmath>
#include "StellarCartography/StarMap.h"

using namespace StellarCartography;

SC_TEST_SUITE(StarMapTests)

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

StarMap basicGalaxy()
{
    return 
    {
        sol(),
        proximaCentauri(),
        alphaCentauri(),
        polaris(),
        sirius(),
        betaCanisMajoris(),
    };
}

SC_TEST_CASE(StarMapTests, TestCopyCtor)
{
    auto h = StarMap();
    {
        auto g = basicGalaxy();
        h = StarMap(g); 
    }

    BOOST_REQUIRE_EQUAL(basicGalaxy().size(), h.size());
    h.nearestNeighbor(sol(), 10.0);
}

SC_TEST_CASE(StarMapTests, TestGetStar)
{
    auto g = basicGalaxy();

    BOOST_CHECK_EQUAL(sol(), g.getStar(sol().getName()));
    BOOST_CHECK_EQUAL(sirius(), g.getStar(sirius().getName()));

    BOOST_CHECK_THROW(g.getStar(Star().getName()), std::invalid_argument);
    BOOST_CHECK_THROW(g.getStar("Foo"), std::invalid_argument);
}

SC_TEST_CASE(StarMapTests, TestGetEdges)
{
    auto g = basicGalaxy();

    BOOST_CHECK_EQUAL(g.size() * (g.size() - 1) / 2, num_edges(g));
    
    auto e = edges(g);
    std::vector<Jump> edge_vec(e.first, e.second);
    
    BOOST_CHECK_EQUAL(edge_vec.size(), num_edges(g));
}

SC_TEST_CASE(StarMapTests, TestNearestNeighbor)
{
    StarMap g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        proximaCentauri(), g.nearestNeighbor(sol().getName(), 5.0));
    BOOST_CHECK_EQUAL(
        betaCanisMajoris(), g.nearestNeighbor(alphaCentauri().getName(), 7.0)
    );
    BOOST_CHECK_EQUAL(
        Star(), g.nearestNeighbor(sirius(), 1.0)
    );
}

SC_TEST_CASE(StarMapTests, TestNeighbors)
{
    StarMap g = basicGalaxy();

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

SC_TEST_CASE(StarMapTests, TestPath)
{
    StarMap g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (StarList { sol(), proximaCentauri() }),
        (g.path(sol(), proximaCentauri(), 5))
    );

    BOOST_CHECK_EQUAL(
        (StarList { sol(), proximaCentauri(), alphaCentauri(), betaCanisMajoris() }),
        (g.path(sol().getName(), betaCanisMajoris().getName(), 10))
    );
}

SC_TEST_CASE(StarMapTests, TestRoutingWeight)
{
    /* 
     * Verify we take the path with the fewest jumps, not least
     * total distance.
     */
    Star 
        a { "A", { 0, 0, 0 } },
        b { "B", { 1, 0, 0 } },
        c { "C", { 2, 0, 0 } },
        d { "D", { 3, 0, 0 } },
        e { "E", { 1.5, 0.5, 0 } };

    StarMap g { a, b, c, d, e };

    
    BOOST_CHECK_EQUAL(
        (StarList { a, e, d }),
        (g.path(a, d, 1.99))
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

SC_TEST_CASE(StarMapTests, TestReachable)
{
    StarMap g = basicGalaxy();

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

SC_TEST_CASE(StarMapTests, TestConnectedComponents)
{
    StarMap g = basicGalaxy();

    BOOST_CHECK_EQUAL(
        (
            std::list<StarSet> 
            {
                { alphaCentauri(), betaCanisMajoris() },
                { sol(), proximaCentauri(), polaris() },
                { sirius() }
            }
        ),
        g.connectedComponents(7.0)
    );
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
