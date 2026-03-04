#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
//-------------------
#include "types.h"
#include "orderbook.h"
#include "utility.h"

const std::string ORDERS_PATH = "../order-book/data/orders.csv";

bool add_order(order_book& orderbook, const order_data& order)
{
    if (order.quantity == 0)
    {
        std::cout << "SYSTEM: can't add an order with quantity 0" << std::endl;
        return false;
    }
    
    if (orderbook.order_index.contains(order.id))
        return false;

    auto& sidebook = (order.side == side::BUY) ? orderbook.bidbook : orderbook.askbook;
    
    // get iterator to an inserted list of orders
    auto [lvlit, inserted] = sidebook.try_emplace(order.price);
    
    // move order into the list
    lvlit->second.emplace_back(order);

    // get iterator to the last order in the list
    auto orderIt = std::prev(lvlit->second.end());
    
    // insert location of the order in the map
    order_data& insertedOrder = lvlit->second.back();
    orderbook.order_index[insertedOrder.id] =
        {
            .price_level_iterator= lvlit,
            .order_iterator= orderIt,
            .side = insertedOrder.side,
        };

    return inserted;
}


u64 match_orders(order_book& orderbook, order_data& order)
{
    u64 matches = 0;
    auto& askbook = orderbook.askbook;
    auto& bidbook = orderbook.bidbook;

    if (order.side == side::BUY)
    {
        while (order.quantity > 0 && !askbook.empty())
        {
            auto it = askbook.begin();
            auto& [bestAskPrice, queue] = *it;

            if (order.price < bestAskPrice)
                break;

            while (order.quantity > 0 && !queue.empty())
            {
                auto& resting = queue.front();
                u64 traded = std::min(order.quantity, resting.quantity);

                order.quantity -= traded;
                resting.quantity -= traded;
                matches += traded;

                if (resting.quantity == 0)
                {
                    orderbook.order_index.erase(resting.id);
                    queue.pop_front();
                }
            }

            if (queue.empty())
                askbook.erase(it);
        }

        if (order.quantity > 0)
        {
            add_order(orderbook, order);
        }
    }
    else
    {
        while (order.quantity > 0 && !bidbook.empty())
        {
            auto it = std::prev(bidbook.end());
            auto& [bestBidPrice, queue] = *it;

            if (order.price > bestBidPrice)
                break;

            while (order.quantity > 0 && !queue.empty())
            {
                order_data& resting = queue.front();
                u64 traded = std::min(order.quantity, resting.quantity);

                order.quantity -= traded;
                resting.quantity -= traded;
                matches += traded;

                if (resting.quantity == 0)
                {
                    orderbook.order_index.erase(resting.id);
                    queue.pop_front();
                }
            }

            if (queue.empty())
                bidbook.erase(it);
        }

        if (order.quantity > 0)
        {
            add_order(orderbook, order);
        }
    }

    return matches;
};

bool cancel_order(order_book& orderbook, order_data& order)
{
    auto it = orderbook.order_index.find(order.id);
    if (it == orderbook.order_index.end())
    {
        return false;
    }
    
    order_location& orderLocation = it->second;
    auto& sidebook = (orderLocation.side == side::BUY) ? orderbook.bidbook : orderbook.askbook;

    auto& queue = orderLocation.price_level_iterator->second;
    std::cout << "SYSTEM: canceled order with id: " << orderLocation.order_iterator->id << std::endl;
    queue.erase(orderLocation.order_iterator);
    
    if (queue.empty())
    {
        sidebook.erase(orderLocation.price_level_iterator);
    }

    orderbook.order_index.erase(it);
    return true;
};

order_data parse_order_from_file(const std::string& line)
{
    char delimiter = ',';
    std::stringstream ss(line);
    std::string sOperation, sId, sPrice, sQuantity, sSide;

    std::getline(ss, sOperation, delimiter);
    std::getline(ss, sId, delimiter);
    std::getline(ss, sPrice, delimiter);
    std::getline(ss, sQuantity, delimiter);
    std::getline(ss, sSide, delimiter);
    
    order_data order 
    {
        .price = static_cast<u32>(std::round(string_to_double(sPrice) * 100)),
        .quantity = static_cast<u32>(string_to_int(sQuantity)),
        .id = static_cast<u32>(string_to_int(sId)),

        // TO DO: Figure out a better way to parse strings -> enums
        .side = (sSide == "Buy" || sSide == "BUY") ? side::BUY : side::SELL,
        .operation = (sOperation == "Add" || sOperation == "ADD") ? operation::ADD : operation::CANCEL,
    };
   
    return order;
};

int main()
{
    order_book orderbook;
    u32 matches = 0;

    std::ifstream file(ORDERS_PATH);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "SYSTEM: failed to open a file \n";
        return EXIT_FAILURE;
    }

    while (std::getline(file, line))
    {
        char firstCharacter = line[0];
        if (firstCharacter != '#')
        {
            order_data order = parse_order_from_file(line);
            
            switch(order.operation)
            {
                case operation::ADD: matches += match_orders(orderbook, order); break;
                case operation::CANCEL: cancel_order(orderbook, order); break;
                default: assert(false && "Unhandeled Operation - No Implementation"); break;
            }
        }
    }

    file.close();
    
    print_book(orderbook.bidbook, "BUY ORDERS: ");
    print_book(orderbook.askbook, "SELL ORDERS: ");

    std::cout << "\nTotal Matched Orders: " << matches << "\n\n";
    
    return EXIT_SUCCESS;
}
