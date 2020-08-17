#include <iostream>
#include <string>

#include "MenuItem.h"

void MenuItem::printMenuItem()
{
    std::cout << _name << "  $" << _price;
}

std::string MenuItem::getName()
{
    return _name;
}

int MenuItem::getPrice()
{
    return _price;
}