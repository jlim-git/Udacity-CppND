#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>

class MenuItem
{
public:
  MenuItem(std::string name, int price) : _name(name), _price(price) {};

  void printMenuItem();
  std::string getName();
  int getPrice();

private:
  std::string _name;
  int _price;
};

#endif