#include "Tests.h"

#include "StellarCartography/Star.h"

using namespace StellarCartography;

SC_TEST_SUITE(StarTests)

SC_TEST_CASE(StarTests, Basic)
{
    Star s;

    BOOST_CHECK_EQUAL(s, s);

    Star a { "Foo", { 0.0, 1.0, 2.0 } };

    BOOST_CHECK_EQUAL("Foo", a.getName());
    BOOST_CHECK_EQUAL(
        (Coordinate {0.0, 1.0, 2.0}),
        a.getCoords()
    );

    BOOST_CHECK_THROW(a.getProperty("flibbety floo"), std::invalid_argument);

    a.setProperty("foo", "bar");
    BOOST_CHECK_EQUAL("bar", a.getProperty("foo"));
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
