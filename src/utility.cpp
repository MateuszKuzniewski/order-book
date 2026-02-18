#include <cstddef>
#include <iostream>
//------------------
#include "utility.h"

void PrintDeque(const std::deque<Order>& d)
{
    if (d.empty())
        std::cout << "List is empty\n";

    std::cout << "quantity: ";
    for (size_t i = 0; i < d.size(); i++)
    {
        if (i == 0)
        {
            std::cout << d[i].quantity;       
        }

        std::cout << ", "<< d[i].quantity;
    }
    std::cout << "\n";
};

void PrintMap(const OrderMap& map, const std::string& text)
{
    std::cout << text << "\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << "\n Order "; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};
