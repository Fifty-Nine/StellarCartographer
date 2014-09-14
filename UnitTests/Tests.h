#ifndef TESTS_H
#define TESTS_H

#include "StellarCartography/All.h"

#include <boost/test/unit_test.hpp>

#define SC_TEST_SUITE(suite) BOOST_AUTO_TEST_SUITE(suite)
#define SC_TEST_SUITE_END() BOOST_AUTO_TEST_SUITE_END()
#define SC_TEST_CASE(suite, name) BOOST_AUTO_TEST_CASE(name)
#define SC_TEST_CASE_END()

#endif /* TESTS_H */
