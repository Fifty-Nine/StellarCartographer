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

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarSet { polaris(), proximaCentauri() }),
        (g.neighbors(sol(), 6.0))
    );

    SC_CHECK_EQUAL_COLLECTIONS(
        StarSet(),
        (g.neighbors(sol().getName(), 1.0))
    );

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarSet { alphaCentauri() }),
        (g.neighbors(betaCanisMajoris(), 7))
    );
}

SC_TEST_CASE(StarMapTests, TestPath)
{
    StarMap g = basicGalaxy();

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarList { sol(), proximaCentauri() }),
        (g.path(sol(), proximaCentauri(), 5))
    );

    SC_CHECK_EQUAL_COLLECTIONS(
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

    
    SC_CHECK_EQUAL_COLLECTIONS(
        (StarList { a, e, d }),
        (g.path(a, d, 1.99))
    );

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarList { a, d }),
        (g.path(a, d, 4.0))
    );
    
    SC_CHECK_EQUAL_COLLECTIONS(
        (StarList { a, b, c, d }),
        (g.path(a, d, 1.1))
    );
}
SC_TEST_CASE_END()

SC_TEST_CASE(StarMapTests, TestReachable)
{
    StarMap g = basicGalaxy();

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarSet { sol(), proximaCentauri(), polaris() }),
        g.reachable(sol(), 5.1)
    );

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarSet { alphaCentauri(), betaCanisMajoris() }),
        (g.reachable(alphaCentauri().getName(), 7.0))
    );

    SC_CHECK_EQUAL_COLLECTIONS(
        (StarSet { alphaCentauri(), betaCanisMajoris() }),
        (g.reachable(betaCanisMajoris(), 7.0))
    );
}
SC_TEST_CASE_END()

SC_TEST_CASE(StarMapTests, TestConnectedComponents)
{
    StarMap g = basicGalaxy();
    auto exp = std::vector<StarSet>
    {
        { sol(), proximaCentauri(), polaris() },
        { alphaCentauri(), betaCanisMajoris() },
        { sirius() }
    };
    auto act = g.connectedComponents(7.0);

    BOOST_REQUIRE_EQUAL(exp.size(), act.size());
    auto it = exp.begin();
    auto jt = act.begin();
    for (; it != exp.end() && jt != act.end(); ++it, ++jt)
    {
        SC_CHECK_EQUAL_COLLECTIONS(*it, *jt);
    }
}
SC_TEST_CASE_END()

SC_TEST_CASE(StarMapTests, TestByDistance)
{
    StarMap g = basicGalaxy();

    auto d = g.byDistance(10.0);

    BOOST_CHECK_EQUAL(num_vertices(g), num_vertices(d));
    
    auto v = vertices(d);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        g.begin(), g.end(), 
        v.first, v.second
    );

}
SC_TEST_CASE_END()

SC_TEST_CASE(StarMapTests, TestEdges)
{
    Star 
        a { "a", { 0.0, 0.0, 0.0 } },
        b { "b", { 1.0, 0.0, 0.0 } },
        c { "c", { 0.0, 1.0, 0.0 } },
        d { "d", { 1.0, 1.0, 0.0 } };

    StarMap g { a, b, c, d };
    auto idx = g.byDistance(1.1);

    JumpSet e(edges(idx).first, edges(idx).second);

    BOOST_CHECK_EQUAL(4, num_edges(idx));
    BOOST_REQUIRE_EQUAL(e.size(), num_edges(idx));
    SC_CHECK_EQUAL_COLLECTIONS(
        (JumpList { { a, b }, { a, c }, { b, d }, { c, d } }), 
        e
    );

    auto idx2 = g.byDistance(2);

    auto g_edges = JumpSet(edges(g).first, edges(g).second);
    e = JumpSet(edges(idx2).first, edges(idx2).second);

    BOOST_CHECK_EQUAL(num_edges(g), num_edges(idx2));
    SC_CHECK_EQUAL_COLLECTIONS(g_edges, e);

    BOOST_CHECK_EQUAL(2, out_degree(a, idx));
    BOOST_CHECK_EQUAL(2, out_degree(b, idx));
    BOOST_CHECK_EQUAL(2, out_degree(c, idx));
    BOOST_CHECK_EQUAL(2, out_degree(d, idx));

    BOOST_CHECK_EQUAL(3, out_degree(a, idx2));
    BOOST_CHECK_EQUAL(3, out_degree(b, idx2));
    BOOST_CHECK_EQUAL(3, out_degree(c, idx2));
    BOOST_CHECK_EQUAL(3, out_degree(d, idx2));

    auto a_out = JumpSet(out_edges(a, idx).first, out_edges(a, idx).second);
    SC_CHECK_EQUAL_COLLECTIONS(
        (JumpList { { a, b }, { a, c } }),
        a_out
    );

    a_out = JumpSet(out_edges(a, idx2).first, out_edges(a, idx2).second);
    SC_CHECK_EQUAL_COLLECTIONS(
        (JumpList { { a, b }, { a, c }, { a, d } }),
        a_out
    );
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
