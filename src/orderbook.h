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
    u32 price;
    u32 quantity;
    u32 id;
    Side side;
};

// mapping: price, list of orders for that price
typedef std::map<u32, std::deque<Order>> OrderMap;

struct OrderBook
{
    OrderMap bidBook;
    OrderMap askBook;
};
