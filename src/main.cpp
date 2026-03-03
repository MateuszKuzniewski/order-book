#include <cstdlib>
#include <cmath>
#include <iostream>
#include <array>
#include <fstream>
#include <assert.h>
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
        std::cout << "can't add an order with quantity 0" << std::endl;
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


u64 MatchOrders(OrderBook& orderBook, std::array<Order, 5>& orders)
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
        return false;
    }
    
    OrderLocation& orderLocation = it->second;
    auto& sideBook = (orderLocation.side == Side::BUY) ? orderBook.bidBook : orderBook.askBook;

    auto& queue = orderLocation.priceLevelIterator->second;
    std::cout << "canceled order with id: " << orderLocation.orderIterator->id << std::endl;
    queue.erase(orderLocation.orderIterator);
    
    if (queue.empty())
    {
        sideBook.erase(orderLocation.priceLevelIterator);
    }

    orderBook.orderIndex.erase(it);
    return true;
};

void ParseOrderFromFile(const std::string& line, Order& order)
{
    std::stringstream ss(line);
    std::string sCommand, sId, sPrice, sQuantity, sSide;
    std::getline(ss, sCommand, ',');
    std::getline(ss, sId, ',');
    std::getline(ss, sPrice, ',');
    std::getline(ss, sQuantity, ',');
    std::getline(ss, sSide, ',');
    
    order.id = static_cast<u32>(StringToInt(sId));
    order.quantity = static_cast<u32>(StringToInt(sQuantity));
    order.price = static_cast<u32>(std::round(StringToDouble(sPrice) * 100));
             
    order.command = (sCommand == "Add" || sCommand == "ADD") ? Command::ADD : Command::CANCEL;
    order.side = (sSide == "Buy" || sSide == "BUY") ? Side::BUY : Side::SELL;

    // std::cout << "command: " << (int)order.command 
    //     << " id: " << order.id
    //     << " price: " << order.price 
    //     << " quantity: " << order.quantity 
    //     << " side: " << (int)order.side << std::endl;
};

int main()
{
    OrderBook ob;
    std::ifstream file(ORDERS_PATH);
    
    if (!file.is_open())
    {
        std::cout << "Failed to open a file \n";
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line[0] != '#')
        {
            Order order {};
            ParseOrderFromFile(line, order);
            
            if (order.command == Command::ADD)
                AddOrder(ob, order);
            else
                CancelOrder(ob, order.id);
        }
    }

    file.close();
    
    std::cout << "-------------- BEFORE ---------------\n";

    PrintBook(ob.bidBook, "BUY ORDERS: ");
    PrintBook(ob.askBook, "SELL ORDERS: ");


    std::cout << "\n-------------- AFTER ---------------\n";
    PrintBook(ob.bidBook, "BUY ORDERS: ");
    PrintBook(ob.askBook, "SELL ORDERS: ");

    // std::cout << "\nTotal Matched Orders: " << matches << std::endl;
    
    return EXIT_SUCCESS;
}
