#ifndef MENU_H
#define MENU_H

#include <vector>
#include <memory>
#include <string>

#include "MenuItem.h"

class Menu
{
public:
    Menu(std::string filepath)
    {
        readFromFile(filepath);
    };

    std::shared_ptr<MenuItem> getMenuItem(int itemIndex);
    int getSize();

    void readFromFile(std::string filepath);
    void addItem(std::shared_ptr<MenuItem> menuItem);

    void printMenu();
    void printMenuItem(int index);

private:
    std::vector<std::shared_ptr<MenuItem>> _menu;
};

#endif