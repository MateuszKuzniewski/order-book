#pragma once
#include <string>
//----------------
#include "orderbook.h"


void PrintDeque(const std::deque<Order>& d);
void PrintBook(const OrderMap& map, const std::string& text);
u32 GenerateID();
