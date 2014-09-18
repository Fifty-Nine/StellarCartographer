#ifndef TESTS_H
#define TESTS_H

#include "StellarCartography/All.h"

#include <boost/test/unit_test.hpp>

#define SC_TEST_SUITE(suite) BOOST_AUTO_TEST_SUITE(suite)
#define SC_TEST_SUITE_END() BOOST_AUTO_TEST_SUITE_END()
#define SC_TEST_CASE(suite, name) BOOST_AUTO_TEST_CASE(name)
#define SC_TEST_CASE_END()

namespace StellarCartography
{

std::ostream& operator<<(
    std::ostream& os, const StellarCartography::Coordinate& c);
std::ostream& operator<<(
    std::ostream& os, const StellarCartography::Star& c);

}

#define SC_CHECK_EQUAL_COLLECTIONS(exp, act) \
do { \
    auto actv = (act); \
    auto expv = (exp); \
    BOOST_CHECK_EQUAL_COLLECTIONS( \
        expv.begin(), expv.end(), \
        actv.begin(), actv.end() \
    ); \
} while(0)

#endif /* TESTS_H */
