#include "Tests.h"

#include "StellarCartography/Algorithms.h"

using namespace StellarCartography;

SC_TEST_SUITE(AlgorithmTests)

namespace
{

double error(const Coordinate& p1, const Coordinate& p2)
{
    return p1.distance(p2);
}

}

SC_TEST_CASE(AlgorithmTests, TrilaterateTests)
{
    typedef std::pair<Coordinate, double> Arg;
    typedef std::vector<Arg> ArgList;

    ArgList args {
        { { 2.0, 1.0, 1.0 }, 1.0 },
        { { 1.0, 2.0, 1.0 }, 1.0 },
        { { 1.0, 1.0, 2.0 }, 1.0 }
    };

    sort(args.begin(), args.end());

    Coordinate p = { 1.0, 1.0, 1.0 };

    BOOST_CHECK_THROW(
        trilaterate(args.begin(), args.begin()), std::length_error);

    while (std::next_permutation(args.begin(), args.end()))
    {
        Coordinate q = trilaterate(args.begin(), args.end());
        BOOST_CHECK_CLOSE(1.0, 1.0+error(p, q), 0.001);
    }
}
SC_TEST_CASE_END()

SC_TEST_SUITE_END()
