#include <iostream>
#include <string>
#include <memory>

#include "Menu.h"
#include "MenuItem.h"
#include "FOH.h"

/*

TODO
- Update CMakeLists to copy sampleMenu.csv from ./src to ./build

*/

void printWelcome()
{
    std::cout << "============================================"
              << "\n"
              << "\n"
              << "CppND Capstone: Restaurant Management System"
              << "\n"
              << "\n"
              << "============================================"
              << "\n";
}

void printCmd()
{
    std::cout << "\n"
              << "Commands:"
              << "\n"
              << "1 = New Table"
              << "\n"
              << "2 = Add Order To Table"
              << "\n"
              << "3 = Checkout Table"
              << "\n"
              << "4 = End Service (Close Program)"
              << "\n"
              << "\n"
              << "Enter Command: ";
}

int main()
{
    printWelcome();

    FOH FOH; // new front of house agent

    auto todaysMenu = std::make_shared<Menu>("sampleMenu.txt"); // load menu from csv

    while (true)
    {
        int userInput;
        printCmd();
        std::cin >> userInput;

        if (userInput == 1) // add New Table
        {
            FOH.newTable();
        }

        else if (userInput == 2) // add Order to Table
        {
            FOH.addOrder(todaysMenu);
        }

        else if (userInput == 3) // checkout Table
        {
            int closeTable = FOH.selectOpenTable("Checkout Table #: ");
            FOH.checkout(closeTable);
        }

        else if (userInput == 4) // end service
        {
            FOH.endService();
            break;
        }

        else
        {
            std::cout << "\n"
                      << "Unknown Command"
                      << "\n";
        }
    }

    return 0;
}