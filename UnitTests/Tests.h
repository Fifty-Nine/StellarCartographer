#ifndef TESTS_H
#define TESTS_H

#include "StellarCartography/All.h"

#include <boost/test/unit_test.hpp>

#define SC_TEST_SUITE(suite) BOOST_AUTO_TEST_SUITE(suite)
#define SC_TEST_SUITE_END() BOOST_AUTO_TEST_SUITE_END()
#define SC_TEST_CASE(suite, name) BOOST_AUTO_TEST_CASE(name)
#define SC_TEST_CASE_END()

namespace boost 
{
namespace test_tools
{

template<> 
struct print_log_value<StellarCartography::Coordinate> 
{
    void operator()(std::ostream& os, const StellarCartography::Coordinate& c);
};

template<>
struct print_log_value<StellarCartography::Star>
{
    void operator()(std::ostream& os, const StellarCartography::Star& c);
};

template<>
struct print_log_value<StellarCartography::StarList>
{
    void operator()(
        std::ostream& os, const StellarCartography::StarList& sl);
};

template<>
struct print_log_value<StellarCartography::StarSet>
{
    void operator()(
        std::ostream& os, const StellarCartography::StarSet& sl);
};

template<>
struct print_log_value<std::list<StellarCartography::StarSet>>
{
    void operator()(
        std::ostream& os, const std::list<StellarCartography::StarSet>& sl);
};

}
}

#endif /* TESTS_H */
