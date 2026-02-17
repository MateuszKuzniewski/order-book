#pragma once
#include <string>
//----------------
#include "orderbook.h"


void PrintDeque(const std::deque<Order>& d);
void PrintMap(const std::map<u32, std::deque<Order>, std::greater<u32>>& map, const std::string& text);
void PrintMap(const std::map<u32, std::deque<Order>, std::less<u32>>& map, const std::string& text);
