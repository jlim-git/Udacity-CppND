#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>

#include "Menu.h"
#include "MenuItem.h"

void Menu::readFromFile(std::string filepath)
{
    std::vector<std::vector<std::string>> menu;

    std::fstream menuFile;
    menuFile.open(filepath);

    if (menuFile.is_open())
    {
        std::string line, val, name, price;

        while (std::getline(menuFile, line, '\n'))
        {
            std::stringstream row(line);
            std::vector<std::string> v;

            while (std::getline(row, val, ','))
            {
                v.emplace_back(val);
            }

            menu.emplace_back(v);
        }
    }

    for (auto i : menu)
    {
        std::string::size_type sz;
        auto newItem = std::make_shared<MenuItem>(i[0], std::stoi(i[1], &sz));
        addItem(std::move(newItem));
    }
}

int Menu::getSize()
{
    return _menu.size();
}

void Menu::addItem(std::shared_ptr<MenuItem> menuItem)
{
    _menu.emplace_back(menuItem);
}

void Menu::printMenu()
{
    std::cout << "\n"
              << "Today's Menu:"
              << "\n";

    for (int i = 0; i < _menu.size(); i++)
    {
        std::cout << i + 1 << ") ";
        _menu[i]->printMenuItem();
        std::cout << "\n";
    }
}

void Menu::printMenuItem(int index)
{
    _menu[index]->printMenuItem();
}

std::shared_ptr<MenuItem> Menu::getMenuItem(int itemIndex)
{
    auto menuItem = _menu[itemIndex];
    return menuItem;
}