#include <iostream>
//---------------------
#include "orderbook.h"

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


