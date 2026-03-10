#define CATCH_CONFIG_MAIN
#include <orderbook.h>
#include <utility.h>
#include <types.h>
#include <order_parser.h>
//------------------------
#include "catch.h"

TEST_CASE("convert string to int", "[stringtoint]") 
{
    REQUIRE(string_to_int("1") == 1);
    REQUIRE(string_to_int("15012592") == 15012592);
    REQUIRE(string_to_int("") == 0);
    REQUIRE(string_to_int(" ") == 0);
    REQUIRE(string_to_int("0") == 0);
}

TEST_CASE("convert string to double", "[stringtodouble]")
{
    REQUIRE(string_to_double("1.1") == 1.1);
    REQUIRE(string_to_double("1000000.01") == 1000000.01);
    REQUIRE(string_to_double("") == 0);
    REQUIRE(string_to_double(" ") == 0);
    REQUIRE(string_to_double("0") == 0);
    REQUIRE(string_to_double("0.0") == 0);
}

TEST_CASE("parse string into an order" "[parseorderfromfile]")
{
    const std::string testString = "ADD,1,101.5,100,SELL";
    const order_data testOrder 
    {
        .price = 10150,
        .quantity = 100,
        .id = 1,

        // TO DO: Figure out a better way to parse strings -> enums
        .side = side::SELL,
        .operation = operation::ADD,
    };
 
    REQUIRE(parse_order_from_file(testString) == testOrder);
}
