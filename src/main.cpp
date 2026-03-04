#include <cmath>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
//-------------------
#include "types.h"
#include "orderbook.h"
#include "utility.h"

const std::string ORDERS_PATH = "../order-book/data/orders.csv";

bool AddOrder(OrderBook& orderBook, const Order& order)
{
    if (order.quantity == 0)
    {
        std::cout << "SYSTEM: can't add an order with quantity 0" << std::endl;
        return false;
    }
    
    if (orderBook.orderIndex.contains(order.id))
        return false;

    auto& sidebook = (order.side == Side::BUY) ? orderBook.bidBook : orderBook.askBook;
    
    // get iterator to an inserted list of orders
    auto [lvlit, inserted] = sidebook.try_emplace(order.price);
    
    // move order into the list
    lvlit->second.emplace_back(order);

    // get iterator to the last order in the list
    auto orderIt = std::prev(lvlit->second.end());
    
    // insert location of the order in the map
    Order& insertedOrder = lvlit->second.back();
    orderBook.orderIndex[insertedOrder.id] =
        {
            .priceLevelIterator = lvlit,
            .orderIterator = orderIt,
            .side = insertedOrder.side,
        };

    return inserted;
}


u64 MatchOrders(OrderBook& orderBook, Order& order)
{
    u64 matches = 0;
    auto& askBook = orderBook.askBook;
    auto& bidBook = orderBook.bidBook;

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

    return matches;
};

bool CancelOrder(OrderBook& orderBook, Order& order)
{
    auto it = orderBook.orderIndex.find(order.id);
    if (it == orderBook.orderIndex.end())
    {
        return false;
    }
    
    OrderLocation& orderLocation = it->second;
    auto& sideBook = (orderLocation.side == Side::BUY) ? orderBook.bidBook : orderBook.askBook;

    auto& queue = orderLocation.priceLevelIterator->second;
    std::cout << "SYSTEM: canceled order with id: " << orderLocation.orderIterator->id << std::endl;
    queue.erase(orderLocation.orderIterator);
    
    if (queue.empty())
    {
        sideBook.erase(orderLocation.priceLevelIterator);
    }

    orderBook.orderIndex.erase(it);
    return true;
};

Order ParseOrderFromFile(const std::string& line)
{
    Order order {};
    char delimiter = ',';
    std::stringstream ss(line);
    std::string sOperation, sId, sPrice, sQuantity, sSide;

    std::getline(ss, sOperation, delimiter);
    std::getline(ss, sId, delimiter);
    std::getline(ss, sPrice, delimiter);
    std::getline(ss, sQuantity, delimiter);
    std::getline(ss, sSide, delimiter);
    
    order.id = static_cast<u32>(StringToInt(sId));
    order.quantity = static_cast<u32>(StringToInt(sQuantity));
    order.price = static_cast<u32>(std::round(StringToDouble(sPrice) * 100));

    // TO DO: Figure out a better way to parse strings -> enums
    order.operation = (sOperation == "Add" || sOperation == "ADD") ? Operation::ADD : Operation::CANCEL;
    order.side = (sSide == "Buy" || sSide == "BUY") ? Side::BUY : Side::SELL;
    
    return order;
};

int main()
{
    OrderBook ob;
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
        if (line[0] != '#')
        {
            Order order = ParseOrderFromFile(line);
            
            if (order.operation == Operation::ADD)
                matches += MatchOrders(ob, order);
            else
                CancelOrder(ob, order);
        }
    }

    file.close();
    
    PrintBook(ob.bidBook, "BUY ORDERS: ");
    PrintBook(ob.askBook, "SELL ORDERS: ");

    std::cout << "\nTotal Matched Orders: " << matches << "\n\n";
    
    return EXIT_SUCCESS;
}
