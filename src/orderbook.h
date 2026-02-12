#pragma once
#include <deque>
#include <map>
//----------------
#include "types.h"


enum class Side : u8
{
    BUY,
    SELL
};

struct Order
{
    u64 price;
    u64 quantity;
    Side side;
};

struct OrderBook
{
    // maps price to a list of orders
    // structure: price, order, sort-by less/more
    std::map<u64, std::deque<Order>, std::greater<u64>> bidBook;
    std::map<u64, std::deque<Order>, std::less<u64>> askBook;
};
