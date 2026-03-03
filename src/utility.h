#pragma once
#include <string>
//----------------
#include "orderbook.h"

i32 StringToInt(const std::string& string);
f64 StringToDouble(const std::string& string);
void PrintDeque(const std::deque<Order>& d);
void PrintBook(const OrderMap& map, const std::string& text);
u32 GenerateID();
