#include <iostream>
#include <fstream>
#include <assert.h>
//-------------------
#include "types.h"
#include "utility.h"
#include "order_parser.h"
#include "orderbook.h"

int main()
{
    order_book orderbook;
    const std::string ORDERS_PATH = "../order-book/data/orders.csv";

    std::ifstream file(ORDERS_PATH);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "SYSTEM: failed to open a file \n";
        return EXIT_FAILURE;
    }
    
    u32 matches = 0;
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
                default: assert(false && "Unhandled Operation - No Implementation"); break;
            }
        }
    }

    file.close();
    
    print_book(orderbook.bidbook, "BUY ORDERS: ");
    print_book(orderbook.askbook, "SELL ORDERS: ");

    std::cout << "\nSYSTEM: total matched orders: " << matches << "\n\n";
    
    return EXIT_SUCCESS;
}
