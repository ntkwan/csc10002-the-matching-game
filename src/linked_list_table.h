#ifndef TABLE_LL_H
#define TABLE_LL_H

#include <time.h>

#include "linked_list.h"
#include "screen.h"

struct TableLL {
    int table_size;
    int padding_left;
    int padding_top;
    int remained_pairs;
    int *character_list;
    int *character_order;
    LinkedList *table_data;
    std::string* table_image;

    TableLL(int, int, int);
    ~TableLL();

    int getXInConsole(int) const;
    int getYInConsole(int) const;

    void generateTableData();
    void displayTableData();
    void loadTableBackground(const std::string &);
    void cleanMatchingEffect();
    void displayTableBackground();
    void printTableData();

};
#endif // TABLE_LL_H
