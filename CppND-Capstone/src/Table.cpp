#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

#include "Table.h"
#include "Menu.h"
#include "MenuItem.h"

int Table::getID()
{
    return _id;
}

int Table::getGuests()
{
    return _guests;
}

int Table::getInvoice()
{
    return _invoice;
}

time_t Table::getTimeIn()
{
    return _timeIn;
}

time_t Table::getTimeOut()
{
    return _timeOut;
}

void Table::setID(int id)
{
    _id = id;
}

void Table::setTimeOut()
{
    _timeOut = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void Table::addOrder(std::shared_ptr<Menu> menu, int itemIndex)
{
    auto newOrder = menu->getMenuItem(itemIndex);
    _invoice += newOrder->getPrice();
    _orders.emplace_back(newOrder);
}

void Table::printReceipt()
{
    std::vector<std::vector<std::string>> collatedOrders = collateOrders();

    std::cout << "\n"
              << "============================================"
              << "\n"
              << "\n"
              << "Time In:  " << ctime(&_timeIn) << "\n"
              << "Time Out: " << ctime(&_timeOut) << "\n";

    std::cout << "Invoice:"
              << "\n";

    for (auto i : collatedOrders)
    {
        std::cout << i[0] << "  $" << i[1] << " each  (Qty " << i[2] << ")"
                  << "\n";
    }

    std::cout << "\n"
              << "Total = $" << _invoice << "\n"
              << "\n"
              << "============================================"
              << "\n"
              << "\n";
}

void Table::sortOrders()
{
    std::sort(_orders.begin(), _orders.end(), [](std::shared_ptr<MenuItem> a, std::shared_ptr<MenuItem> b) {
        return a->getName() < b->getName();
    });
}

std::vector<std::vector<std::string>> Table::collateOrders()
{
    std::vector<std::vector<std::string>> collatedOrders;

    if (_orders.size())
    {
        sortOrders();

        std::string item = _orders[0]->getName();
        int qty = 1, price = _orders[0]->getPrice();

        for (int i = 1; i < _orders.size(); i++)
        {
            if (_orders[i]->getName() == item) // increment qty if same item
            {
                qty++;
            }

            else
            {
                std::vector<std::string> v;
                v.emplace_back(item);
                v.emplace_back(std::to_string(price));
                v.emplace_back(std::to_string(qty));

                collatedOrders.emplace_back(v); // push back {item, price, qty} to collatedOrders

                item = _orders[i]->getName();   // set item to current itemName
                price = _orders[i]->getPrice(); // update price
                qty = 1;                        // reset qty
            }
        }

        std::vector<std::string> v;
        v.emplace_back(item);
        v.emplace_back(std::to_string(price));
        v.emplace_back(std::to_string(qty));

        collatedOrders.emplace_back(v); // push back {item, price, qty} to collatedOrders
    }
    return collatedOrders;
}