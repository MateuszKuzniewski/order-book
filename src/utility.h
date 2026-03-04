#pragma once
#include <string>
//----------------
#include "orderbook.h"

i32 string_to_int(const std::string& string);
f64 string_to_double(const std::string& string);
void print_deque(const std::deque<order_data>& d);
void print_book(const order_map& map, const std::string& text);
