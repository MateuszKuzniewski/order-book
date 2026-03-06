#define CATCH_CONFIG_MAIN
#include <orderbook.h>
#include <utility.h>
#include <types.h>
#include "catch.h"

int TestFunction(int i)
{
    return i;
};

int TestFunction1(int i)
{
    return 5;
};

TEST_CASE("Running test case", "[testfunction]")
{
    REQUIRE(TestFunction(1) == 1);
}

TEST_CASE("Running test case1", "[testfunction1]")
{
    REQUIRE(TestFunction1(1) == 1);
}
