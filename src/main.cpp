#include <deque>
#include <iostream>
#include <map>
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
    u32 id;
    u32 quantity;
};

struct OrderBook
{
    // maps price to a list of orders
    // structure: price, order, sort-by less/more
    std::map<u64, std::deque<Order>, std::greater<i64>> bidBook;
    std::map<u64, std::deque<Order>, std::less<i64>> askBook;
};

void AddOrder(OrderBook& orderBook, const Order& order, Side side)
{
    if (order.quantity > 0)
    {
        switch (side)
        {
            case Side::BUY:  orderBook.bidBook[order.price].push_back(order); break;
            case Side::SELL: orderBook.askBook[order.price].push_back(order); break;
            default: std::cout << "cannot add order\n"; break;
        }
    }
    else
    {
        std::cout << "cannot add order with 0 quantity\n";
    }
};

u64 FindMatch(OrderBook& orderBook)
{
    return 0;
};

void MatchOrders();
void CancelOrder();
void ExecuteOrder();

OrderBook Setup()
{
    OrderBook orderBook;

    AddOrder(orderBook, {120 ,1, 5},  Side::BUY);
    AddOrder(orderBook, {125, 1, 10}, Side::BUY);
    AddOrder(orderBook, {120, 1, 15}, Side::SELL);
    AddOrder(orderBook, {125, 1, 20}, Side::SELL);
    AddOrder(orderBook, {150, 1, 30}, Side::BUY);

    return orderBook;
};

void PrintDeque(const std::deque<Order>& d)
{
    for (auto n : d)
    {
        std::cout << "id: " << n.id << " quantity: " << n.quantity << std::endl;
    }
};


int main()
{
    OrderBook ob = Setup();


    std::cout << "SELL ORDERS: \n\n";
    for (auto it = ob.askBook.begin(); it != ob.askBook.end(); ++it)
    {
        auto index = *it;
        std::cout << "price: " << index.first << " Order: "; 
        PrintDeque(index.second);
    }

    std::cout << "\nBUY ORDERS: \n\n";
    for (auto it = ob.bidBook.begin(); it != ob.bidBook.end(); ++it)
    {
        auto index = *it;
        std::cout << "price: " << index.first << " Order: "; 
        PrintDeque(index.second);
    }
    return 0;
}
