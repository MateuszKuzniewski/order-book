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

bool CanMatch(OrderBook& orderBook, const u64 price, Side side)
{
    const auto& bids = orderBook.bidBook;
    const auto& asks = orderBook.askBook;
    
    if (bids.empty() || asks.empty())
    {
        return false;
    }

    if (side == Side::BUY)
    {
        const auto& [bestAsk, _] = *asks.begin();
        return price >= bestAsk;
    }
    else
    {
        const auto& [bestBid, _] = *bids.begin();
        return price <= bestBid;
    }
};

u32 MatchOrders(OrderBook& orderBook, std::vector<Order>& orders)
{
    for (const auto& order : orders)
    {
        if (!CanMatch(orderBook, order.price, order.side))
        {
            std::cout << "No Matches found" << std::endl;
            break;
        }
    }

    return 0;
};

// void CancelOrder();
// void ExecuteOrder();

OrderBook Setup()
{
    OrderBook orderBook;
    
    std::vector<Order> orders;
    orders.reserve(6);
    orders.push_back({ 120, 5,  Side::BUY  });
    orders.push_back({ 120, 10, Side::BUY  });
    orders.push_back({ 120, 15, Side::SELL });
    orders.push_back({ 120, 20, Side::SELL });
    orders.push_back({ 120, 30, Side::BUY  });
    
    AddOrders(orderBook, orders);

    return orderBook;
};


int main()
{
    OrderBook ob = Setup();
    
    PrintMap(ob.askBook, "BUY ORDERS: ");
    PrintMap(ob.bidBook, "SELL ORDERS: ");

    std::cout << CanMatch(ob, 0, Side::BUY) << std::endl;
    return 0;
}
