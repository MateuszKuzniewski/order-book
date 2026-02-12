#include <deque>
#include <iostream>
#include <map>
#include <vector>
//-------------------
#include "types.h"


enum class Side : u8
{
    BUY,
    SELL
};

struct Order
{
    u64 price;
    u64 quantity;
    Side side;
};

struct OrderBook
{
    // maps price to a list of orders
    // structure: price, order, sort-by less/more
    std::map<u64, std::deque<Order>, std::greater<u64>> bidBook;
    std::map<u64, std::deque<Order>, std::less<u64>> askBook;
};

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
    orders.push_back({ 125, 10, Side::BUY  });
    orders.push_back({ 120, 15, Side::SELL });
    orders.push_back({ 125, 20, Side::SELL });
    orders.push_back({ 150, 30, Side::BUY  });
    
    AddOrders(orderBook, orders);

    return orderBook;
};

void PrintDeque(const std::deque<Order>& d)
{
    for (auto n : d)
    {
        std::cout << " quantity: " << n.quantity << std::endl;
    }
};

void PrintMap(const std::map<u64, std::deque<Order>, std::greater<u64>>map, const std::string& text)
{
    std::cout << text << "\n\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << " Order: "; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};

void PrintMap(const std::map<u64, std::deque<Order>, std::less<u64>>map, const std::string& text)
{
    std::cout << text << "\n\n";
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        const auto& [price, queue] = *it;
        std::cout << "price: " << price << " Order: "; 
        PrintDeque(queue);
    } 
    std::cout << "\n";
};



int main()
{
    OrderBook ob = Setup();
    
    PrintMap(ob.askBook, "BUY ORDERS: ");
    PrintMap(ob.bidBook, "SELL ORDERS: ");

    std::cout << CanMatch(ob, 0, Side::BUY) << std::endl;
    return 0;
}
