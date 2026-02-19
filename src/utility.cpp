#include <cstddef>
#include <iostream>
//------------------
#include "utility.h"

void PrintDeque(const std::deque<Order>& d)
{
    if (d.empty())
    {
        std::cout << "List is empty\n";
        return;
    }

    std::cout << " quantity: ";
    for (size_t i = 0; i < d.size(); i++)
    {
        if (i == 0)
            std::cout << d[i].quantity;       

        std::cout << ", "<< d[i].quantity;
    }
    std::cout << "\n";
};

void PrintMap(const OrderMap& map, const std::string& text)
{
    if (map.empty())
    {
        std::cout << "Map is empty\n";
        return;
    } 
    
    std::cout << text << "\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << "\n Order id: " << queue.begin()->id; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};

u32 GenerateID()
{
    static u32 counter = 1;
    return counter++;
};
