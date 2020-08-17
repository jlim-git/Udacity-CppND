#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>

#include "Menu.h"
#include "MenuItem.h"

class Table
{
public:
    Table(int i) : _guests(i), _invoice(0), _timeIn(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())){};

    int getID();
    int getGuests();
    int getInvoice();
    time_t getTimeIn();
    time_t getTimeOut();

    void setID(int id);
    void setTimeOut();

    void addOrder(std::shared_ptr<Menu> menu, int itemIndex);
    void printReceipt();
    void sortOrders();
    std::vector<std::vector<std::string>> collateOrders();

private:
    int _id;
    int _guests;
    time_t _timeIn;
    time_t _timeOut;
    std::vector<std::shared_ptr<MenuItem>> _orders;
    int _invoice;
};

#endif