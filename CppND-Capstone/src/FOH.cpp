#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

#include "FOH.h"
#include "Table.h"

int FOH::assignID()
{
    int id;

    if (!_openTables.size()) // check if _openTables is empty
    {
        id = 1;
    }

    else
    {
        sortOpenTables();

        for (int n = 0; n < _openTables.size(); n++) // find lowest available integer
        {
            if (n + 1 < _openTables[n]->getID())
            {
                id = n + 1;
                break;
            }

            else if (n == _openTables.size() - 1) // reached end of vector -> assign next highest integer
            {
                id = _openTables[n]->getID() + 1;
            }
        }
    }

    return id;
}

int FOH::selectOpenTable(std::string msg)
{
    if (_openTables.empty()) // check if _openTables is empty
    {
        std::cout << "\n"
                  << "No Open Tables"
                  << "\n";

        return 0;
    }

    else
    {
        std::cout << "\n"
                  << "Open Tables:"
                  << "\n";

        sortOpenTables();

        for (int i = 0; i < _openTables.size(); i++) // print open tables
        {
            std::cout << "Table " << _openTables[i]->getID() << " (" << _openTables[i]->getGuests() << " Guests)"
                      << "\n";
        }

        int selectTable;
        std::cout << "\n"
                  << msg;
        std::cin >> selectTable;

        for (int i = 0; i < _openTables.size(); i++)
        {
            if (_openTables[i]->getID() == selectTable) // find corresponding _openTables index
            {
                return i + 1;
            }

            else if (i == _openTables.size() - 1) // error if ID not found
            {
                std::cout << "Invalid Table Number"
                          << "\n";
                return 0;
            }
        }
    }
}

void FOH::sortOpenTables()
{
    if (_openTables.size() > 1)
    {
        sort(_openTables.begin(), _openTables.end(), [](std::unique_ptr<Table> const &a, std::unique_ptr<Table> const &b) {
            return a->getID() < b->getID();
        });
    }
}

void FOH::newTable()
{
    int guests;

    std::cout << "\n"
              << "Number of Guests: ";
    std::cin >> guests;

    if (guests > 0)
    {
        std::cout << "Finding open table..."
                  << "\n";

        auto assignedTable = std::make_unique<Table>(guests);
        int assignedID = assignID();
        assignedTable->setID(assignedID);
        _openTables.emplace_back(std::move(assignedTable));
        sortOpenTables();

        std::cout << guests << " guests assigned to Table " << assignedID << "\n";
    }

    else
    {
        std::cout << "\n"
                  << "Invalid Number"
                  << "\n";
    }
}

void FOH::addOrder(std::shared_ptr<Menu> menu)
{
    int table = selectOpenTable("Add to Table #: ");

    if (table && menu->getSize())
    {
        int id = _openTables[table - 1]->getID();

        menu->printMenu();

        int item;

        std::cout << "\n"
                  << "Enter item number: ";
        std::cin >> item;

        std::cout << "Menu size = " << menu->getSize() << "\n";

        if (item > 0 && item <= menu->getSize())
        {
            item--;
            _openTables[table - 1]->addOrder(menu, item);

            std::cout << "Added ";
            menu->printMenuItem(item);
            std::cout << " to Table " << id << "\n";
        }

        else
        {
            std::cout << "\n"
                      << "Invalid Menu Item"
                      << "\n";
        }
    }

    return;
}

void FOH::checkout(int closeTable)
{
    if (closeTable)
    {
        closeTable--;

        int tableID = _openTables[closeTable]->getID();
        std::cout << "Table " << tableID << " Closed"
                  << "\n";

        _openTables[closeTable]->setTimeOut();
        _openTables[closeTable]->printReceipt();

        _closedTables.emplace_back(std::move(_openTables[closeTable]));
        _openTables.erase(_openTables.begin() + closeTable);

        sortOpenTables();
    }

    return;
}

void FOH::endService()
{
    std::cout << "Ending Service. Open tables (if any) will be checked out."
              << "\n"
              << "\n";

    if (!_openTables.empty())
    {
        int remainingTables = _openTables.size();
        for (int i = 0; i < remainingTables; i++)
        {
            checkout(_openTables.size());
        }
    }

    if (!_closedTables.empty())
    {

        time_t startTime = _closedTables.front()->getTimeIn(); // timeIn of first table
        time_t endTime = _closedTables.back()->getTimeOut();   // timeOut of last table

        int totalGuests = 0;

        for (int i = 0; i < _closedTables.size(); i++) // sum of guests
        {
            totalGuests += _closedTables[i]->getGuests();
        }

        int totalRevenue = 0;

        for (int i = 0; i < _closedTables.size(); i++) // sum of revenue
        {
            totalRevenue += _closedTables[i]->getInvoice();
        }

        std::cout << "\n"
                  << "\n"
                  << "============================================"
                  << "\n"
                  << "\n"
                  << "Service Start Time: " << ctime(&startTime) << "\n"
                  << "Service End Time:   " << ctime(&endTime) << "\n"
                  << "Total Guests:  " << totalGuests << "\n"
                  << "\n"
                  << "Total Revenue: $" << totalRevenue << "\n"
                  << "\n"
                  << "============================================"
                  << "\n"
                  << "\n";
    }

    else
    {
        std::cout << "\n"
                  << "============================================"
                  << "\n"
                  << "Service Ended"
                  << "\n"
                  << "============================================"
                  << "\n"
                  << "\n";
    }
}
