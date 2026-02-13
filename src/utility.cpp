#include <iostream>
//------------------
#include "utility.h"


void PrintDeque(const std::deque<Order>& d)
{
    for (auto n : d)
    {
        std::cout << " quantity: " << n.quantity << std::endl;
    }
};

void PrintMap(const std::map<u64, std::deque<Order>, std::greater<u64>>& map, const std::string& text)
{
    std::cout << text << "\n\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << " Order: "; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};

void PrintMap(const std::map<u64, std::deque<Order>, std::less<u64>>& map, const std::string& text)
{
    std::cout << text << "\n\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << " Order: "; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};



