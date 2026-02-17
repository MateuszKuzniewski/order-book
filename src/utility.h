#pragma once
#include <string>
//----------------
#include "orderbook.h"


void PrintDeque(const std::deque<Order>& d);
void PrintMap(const OrderMap& map, const std::string& text);
