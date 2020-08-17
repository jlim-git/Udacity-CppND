#ifndef FOH_H
#define FOH_H

#include <vector>
#include <memory>

#include "Table.h"

class FOH
{
public:
    int assignID();
    int selectOpenTable(std::string msg);
    void addOrder(std::shared_ptr<Menu> menu);

    void sortOpenTables();
    void newTable();
    void checkout(int closeTable);
    void endService();

private:
    std::vector<std::unique_ptr<Table>> _openTables;
    std::vector<std::unique_ptr<Table>> _closedTables;
};

#endif