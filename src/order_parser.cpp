#include <cmath>
#include <sstream>
//----------------
#include "order_parser.h"
#include "utility.h"

order_data parse_order_from_file(const std::string& line)
{
    char delimiter = ',';
    std::stringstream ss(line);
    std::string sOperation, sId, sPrice, sQuantity, sSide;

    std::getline(ss, sOperation, delimiter);
    std::getline(ss, sId, delimiter);
    std::getline(ss, sPrice, delimiter);
    std::getline(ss, sQuantity, delimiter);
    std::getline(ss, sSide, delimiter);
    
    order_data order 
    {
        .price = static_cast<u32>(std::round(string_to_double(sPrice) * 100)),
        .quantity = static_cast<u32>(string_to_int(sQuantity)),
        .id = static_cast<u32>(string_to_int(sId)),

        // TO DO: Figure out a better way to parse strings -> enums
        .side = (sSide == "Buy" || sSide == "BUY") ? side::BUY : side::SELL,
        .operation = (sOperation == "Add" || sOperation == "ADD") ? operation::ADD : operation::CANCEL,
    };
   
    return order;
};
