#ifndef TABLE_H
#define TABLE_H

#include <random>
#include <time.h>
#include <iostream>

#include "screen.h"
#include "cell.h"

struct Table {
    int table_size;
    int padding_left;
    int padding_top;
    int remained_pairs;
    int *character_list;
    int *character_order;
    int max_distinct_number = 1;
    int *occurs;
    Cell **table_data;

    Table(int, int, int);
    ~Table();

    int getXInConsole(int) const;
    int getYInConsole(int) const;

    void generateTableData();
    void displayTableData();
    void printTableData();
};

#endif // TABLE_H
