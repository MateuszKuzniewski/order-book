#pragma once
#include <deque>
#include <map>
#include <unordered_map>
//----------------
#include "types.h"

enum class operation : u8
{
    ADD,
    CANCEL
};

enum class side : u8
{
    BUY,
    SELL
};

struct order_data
{
    u32 price;
    u32 quantity;
    u32 id;
    side side;
    operation operation;
};

typedef std::deque<order_data> order_list;

// mapping: price -> list of orders for that price
typedef std::map<u32, order_list> order_map;


struct order_location
{
    order_map::iterator  price_level_iterator;
    order_list::iterator order_iterator;
    side side;
};

// mapping: order id -> order location
typedef std::unordered_map<u32, order_location> order_index;

struct order_book
{
    order_map bidbook;
    order_map askbook;
    order_index order_index;
};
