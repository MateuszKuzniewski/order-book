#pragma once
#include <deque>
#include <map>
#include <unordered_map>
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

typedef std::deque<Order> OrderList;

// mapping: price -> list of orders for that price
typedef std::map<u32, OrderList> OrderMap;


struct OrderLocation
{
    OrderMap::iterator  priceLevelIterator;
    OrderList::iterator orderIterator;
    Side side;
};

// mapping: order id -> order location
typedef std::unordered_map<u64, OrderLocation> OrderIndex;

struct OrderBook
{
    OrderMap bidBook;
    OrderMap askBook;
    OrderIndex orderIndex;
};
