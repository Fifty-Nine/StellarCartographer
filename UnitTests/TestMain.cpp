#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "UnitTests/Tests.h"

struct TestInit
{
    TestInit();
};

TestInit::TestInit()
{
}

BOOST_GLOBAL_FIXTURE(TestInit);
