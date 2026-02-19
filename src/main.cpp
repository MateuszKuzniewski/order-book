#include <iostream>
#include <vector>
#include <assert.h>
//-------------------
#include "types.h"
#include "orderbook.h"
#include "utility.h"

void AddOrder(OrderBook& orderBook, const Order& order)
{
    if (order.quantity == 0)
    {
        std::cout << "couldn't add an order" << std::endl;
        return;
    }
    
    Order resting = order;
    resting.id = GenerateID();

    if (order.side == Side::BUY)
    {
        orderBook.bidBook[order.price].emplace_back(std::move(resting));
    }
    else 
    {
        orderBook.askBook[order.price].emplace_back(std::move(resting));     
    }
}

void AddOrders(OrderBook& orderBook, const std::vector<Order>& orders)
{
    for (const auto& order : orders)
    {
        AddOrder(orderBook, order);
    }
};

u64 MatchOrders(OrderBook& orderBook, std::vector<Order>& orders)
{
    u64 matches = 0;
    auto& askBook = orderBook.askBook;
    auto& bidBook = orderBook.bidBook;

    for (auto& order : orders)
    {
        if (order.quantity == 0)
            continue;
        
        if (order.side == Side::BUY)
        {
            while (order.quantity > 0 && !askBook.empty())
            {
                auto it = askBook.begin();
                auto& [bestAskPrice, queue] = *it;

                if (order.price < bestAskPrice)
                    break;

                while (order.quantity > 0 && !queue.empty())
                {
                    Order& resting = queue.front();
                    u64 traded = std::min(order.quantity, resting.quantity);

                    order.quantity -= traded;
                    resting.quantity -= traded;
                    matches += traded;

                    if (resting.quantity == 0)
                        queue.pop_front();
                }

                if (queue.empty())
                    askBook.erase(it);
            }

            if (order.quantity > 0)
            {
                AddOrder(orderBook, order);
            }
        }
        else
        {
            while (order.quantity > 0 && !bidBook.empty())
            {
                auto it = std::prev(bidBook.end());
                auto& [bestBidPrice, queue] = *it;

                if (order.price > bestBidPrice)
                    break;

                while (order.quantity > 0 && !queue.empty())
                {
                    Order& resting = queue.front();
                    u64 traded = std::min(order.quantity, resting.quantity);

                    order.quantity -= traded;
                    resting.quantity -= traded;
                    matches += traded;

                    if (resting.quantity == 0)
                        queue.pop_front();
                }

                if (queue.empty())
                    bidBook.erase(it);
            }

            if (order.quantity > 0)
            {
                AddOrder(orderBook, order);
            }
        }
    }

    return matches;
};


OrderBook Setup()
{
    OrderBook orderBook;
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 120, 2, 0, Side::BUY  });
    orders.push_back({ 130, 2, 0, Side::BUY  });
    orders.push_back({ 130, 5, 0, Side::BUY  });
    orders.push_back({ 130, 2, 0, Side::SELL });
    orders.push_back({ 120, 2, 0, Side::SELL });
    
    AddOrders(orderBook, orders);

    return orderBook;
};


int main()
{
    OrderBook ob = Setup();
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 110, 1, 0, Side::BUY  });
    orders.push_back({ 130, 1, 0, Side::BUY  });
    orders.push_back({ 150, 1, 0, Side::SELL });
    orders.push_back({ 160, 1, 0, Side::SELL });
    
    std::cout << "-------------- BEFORE ---------------\n";
    PrintMap(ob.bidBook, "BUY ORDERS: ");
    PrintMap(ob.askBook, "SELL ORDERS: ");

    u64 matches = MatchOrders(ob, orders);
    
    std::cout << "\n-------------- AFTER ---------------\n";
    PrintMap(ob.bidBook, "BUY ORDERS: ");
    PrintMap(ob.askBook, "SELL ORDERS: ");

    std::cout << "\n\nTotal Matches: " << matches << std::endl;

    return 0;
}
