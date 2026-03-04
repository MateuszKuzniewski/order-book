#include <cstddef>
#include <iostream>
//------------------
#include "utility.h"

i32 string_to_int(const std::string& string)
{
    i32 fallback = 0;
    if (string.empty())
        return fallback;
    
    return std::stoi(string);
};

f64 string_to_double(const std::string& string)
{
    f64 fallback = 0.0;
    if (string.empty())
        return fallback;

    return std::stod(string);
};

void print_deque(const std::deque<order_data>& d)
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

void print_book(const order_map& map, const std::string& text)
{
    if (map.empty())
    {
        std::cout << text << " Map is empty\n";
        return;
    } 
    
    std::cout << text << "\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << "\n Order id: " << queue.begin()->id;
        print_deque(queue);
    }
    std::cout << "\n";
};
