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

TEST_CASE("parse string into an order", "[parseorderfromfile]")
{
    const std::string testString = "ADD,1,101.5,100,SELL";
    const order_data testOrder 
    {
        .price = 10150,
        .quantity = 100,
        .id = 1,
        .side = side::SELL,
        .operation = operation::ADD,
    };
 
    REQUIRE(parse_order_from_file(testString) == testOrder);
}

TEST_CASE("add order to orderbook", "[addorder]")
{
    order_book ob;

    const order_data testOrder
    {
        .price = 10150,
        .quantity = 100,
        .id = 1,
        .side = side::SELL,
        .operation = operation::ADD
    };

    add_order(ob, testOrder);
    REQUIRE(ob.askbook.begin()->first == testOrder.price);
    REQUIRE(ob.askbook.begin()->second.begin()->quantity == testOrder.quantity);
    REQUIRE(ob.askbook.begin()->second.begin()->id == testOrder.id);
    REQUIRE(ob.askbook.begin()->second.begin()->side == testOrder.side);
    REQUIRE(ob.askbook.begin()->second.begin()->operation == testOrder.operation);
}

TEST_CASE("match orders", "[matchorders]")
{
    order_book ob;
    order_data testOrder1
    {
        .price = 10150,
        .quantity = 100,
        .id = 1,
        .side = side::SELL,
        .operation = operation::ADD
    };

    order_data testOrder2
    {
        .price = 10150,
        .quantity = 100,
        .id = 2,
        .side = side::BUY,
        .operation = operation::ADD
    };

    match_orders(ob, testOrder1);
    match_orders(ob, testOrder2);
    
    REQUIRE(ob.askbook.size() == 0);
    REQUIRE(ob.bidbook.size() == 0);
}

TEST_CASE("cancel order", "[cancelorder]")
{
    order_book ob;
    order_data testOrder
    {
        .price = 10150,
        .quantity = 100,
        .id = 1,
        .side = side::SELL,
        .operation = operation::ADD
    };

    add_order(ob, testOrder);
    cancel_order(ob, testOrder);
    
    REQUIRE(ob.askbook.size() == 0);
    REQUIRE(ob.bidbook.size() == 0);
}
