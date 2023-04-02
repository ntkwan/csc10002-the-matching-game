#ifndef TABLE_LL_H
#define TABLE_LL_H

#include <random>
#include <time.h>
#include <iostream>

#include "screen.h"
#include "linked_list.h"

struct TableLL {
    int table_size_n;
    int table_size_m;
    int padding_left;
    int padding_top;
    int remained_pairs;
    int *character_list;
    int *character_order;
    int max_distinct_number = 2;
    int *occurs;
    LinkedList *table_data;

    TableLL(int, int, int, int);
    ~TableLL();

    int getXInConsole(int) const;
    int getYInConsole(int) const;

    void generateTableData();
    void shuffleTableData();

};
#endif // TABLE_LL_H
