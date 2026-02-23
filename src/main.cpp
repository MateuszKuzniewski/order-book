#include <iostream>
#include <vector>
#include <assert.h>
//-------------------
#include "types.h"
#include "orderbook.h"
#include "utility.h"

bool AddOrder(OrderBook& orderBook, const Order& order)
{
    if (order.quantity == 0)
    {
        std::cout << "couldn't add an order" << std::endl;
        return false;
    }
    
    auto& sidebook = (order.side == Side::BUY) ? orderBook.bidBook : orderBook.askBook;
    
    // get iterator to an inserted list of orders
    auto [lvlit, inserted] = sidebook.try_emplace(order.price);
    
    // move order into the list
    lvlit->second.emplace_back(std::move(order));

    // get iterator to the last order in the list
    auto orderIt = std::prev(lvlit->second.end());
    
    // insert location of the order in the map
    Order& insertedOrder = lvlit->second.back();
    orderBook.orderIndex[insertedOrder.id] =
        {
            lvlit,
            orderIt,
            insertedOrder.side,
        };

    return inserted;
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
                    {
                        orderBook.orderIndex.erase(resting.id);
                        queue.pop_front();
                    }
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
                    {
                        orderBook.orderIndex.erase(resting.id);
                        queue.pop_front();
                    }
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

bool CancelOrder(OrderBook& orderBook, u32 orderID)
{
    auto it = orderBook.orderIndex.find(orderID);
    if (it == orderBook.orderIndex.end())
    {
        std::cout << "Could not cancel order with id: " << orderID << std::endl;
        return false;
    }
    
    OrderLocation& orderLocation = it->second;
    auto& sideBook = (orderLocation.side == Side::BUY) ? orderBook.bidBook : orderBook.askBook;

    auto& queue = orderLocation.priceLevelIterator->second;
    std::cout << "Canceled order with id: " << orderLocation.orderIterator->id << std::endl;
    queue.erase(orderLocation.orderIterator);
    
    if (queue.empty())
    {
        sideBook.erase(orderLocation.priceLevelIterator);
    }

    orderBook.orderIndex.erase(it);
    return true;
};


OrderBook Setup()
{
    OrderBook orderBook;
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 120, 1, GenerateID(), Side::SELL });
    orders.push_back({ 120, 2, GenerateID(), Side::SELL });
    orders.push_back({ 120, 2, GenerateID(), Side::SELL });
    orders.push_back({ 130, 2, GenerateID(), Side::BUY  });
    orders.push_back({ 130, 2, GenerateID(), Side::SELL });
    orders.push_back({ 130, 5, GenerateID(), Side::BUY  });

    AddOrders(orderBook, orders);

    return orderBook;
};


int main()
{
    OrderBook ob = Setup();
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 120, 1, GenerateID(), Side::BUY  });
    orders.push_back({ 130, 1, GenerateID(), Side::BUY  });
    orders.push_back({ 150, 1, GenerateID(), Side::SELL });
    orders.push_back({ 160, 1, GenerateID(), Side::SELL });
    
    std::cout << "-------------- BEFORE ---------------\n";
    PrintBook(ob.bidBook, "BUY ORDERS: ");
    PrintBook(ob.askBook, "SELL ORDERS: ");

    u64 matches = MatchOrders(ob, orders);
    
    std::cout << "\n-------------- AFTER ---------------\n";
    PrintBook(ob.bidBook, "BUY ORDERS: ");
    PrintBook(ob.askBook, "SELL ORDERS: ");

    CancelOrder(ob, 2);
    std::cout << "\nTotal Matched Orders: " << matches << std::endl;

    return 0;
}
