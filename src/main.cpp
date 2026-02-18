#include <iostream>
#include <vector>
//-------------------
#include "types.h"
#include "orderbook.h"
#include "utility.h"


void AddOrders(OrderBook& orderBook, const std::vector<Order>& orders)
{
    for (const auto& order : orders)
    {
        if (order.quantity == 0)
        {
            std::cout << "couldn't add an order" << std::endl;
            continue;
        }

        if (order.side == Side::BUY)
        {
            orderBook.bidBook[order.price].emplace_back(order);
        }
        else 
        {
            orderBook.askBook[order.price].emplace_back(order);     
        }
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
                auto& [bestAskPrice, queue] = *askBook.begin();

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
                    askBook.erase(askBook.begin());
            }

            if (order.quantity > 0)
            {
                bidBook[order.price].emplace_back(order);
            }
        }
        else
        {
            while (order.quantity > 0 && !bidBook.empty())
            {
                auto& [bestBidPrice, queue] = *std::prev(bidBook.end());

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
                    bidBook.erase(std::prev(bidBook.end()));
            }

            if (order.quantity > 0)
            {
                askBook[order.price].emplace_back(order);
            }
        }
    }

    return matches;
};

// void CancelOrder();
// void ExecuteOrder();

OrderBook Setup()
{
    OrderBook orderBook;
    
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 120, 2, Side::BUY  });
    orders.push_back({ 130, 2, Side::BUY  });
    orders.push_back({ 130, 5, Side::BUY  });
    orders.push_back({ 130, 2, Side::SELL });
    orders.push_back({ 120, 2, Side::SELL });
    
    AddOrders(orderBook, orders);

    return orderBook;
};


int main()
{
    OrderBook ob = Setup();
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 110, 1, Side::BUY  });
    orders.push_back({ 130, 1, Side::BUY  });
    orders.push_back({ 150, 1, Side::SELL });
    orders.push_back({ 160, 1, Side::SELL });
    
    std::cout << "-------------- BEFORE ---------------\n";
    PrintMap(ob.askBook, "BUY ORDERS: ");
    PrintMap(ob.bidBook, "SELL ORDERS: ");

    u64 matches = MatchOrders(ob, orders);
    
    std::cout << "\n-------------- AFTER ---------------\n";
    PrintMap(ob.askBook, "BUY ORDERS: ");
    PrintMap(ob.bidBook, "SELL ORDERS: ");

    std::cout << "\n\nTotal Matches: " << matches << std::endl;

    return 0;
}
