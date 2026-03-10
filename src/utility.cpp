#include <cstddef>
#include <iostream>
#include <bits/stdc++.h>
#include <cctype>
//------------------
#include "utility.h"

i32 string_to_int(const std::string& string)
{
    i32 fallback = 0;
    std::string copy = string;

    copy.erase(std::remove_if(copy.begin(), copy.end(), ::isspace), copy.end());

    if (copy.empty())
        return fallback;
    
    return std::stoi(copy);
};

f64 string_to_double(const std::string& string)
{
    i32 fallback = 0;
    std::string copy = string;

    copy.erase(std::remove_if(copy.begin(), copy.end(), ::isspace), copy.end());

    if (copy.empty())
        return fallback;
    
    return std::stod(string);
};

void print_deque(const std::deque<order_data>& d)
{
    if (d.empty())
    {
        std::cout << "list is empty\n";
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
        std::cout << text << "map is empty\n";
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
