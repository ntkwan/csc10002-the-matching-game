#ifndef TABLE_H
#define TABLE_H

#include <random>
#include <time.h>
#include <iostream>

#include "screen.h"
#include "cell.h"

const int CELL_LENGTH = 8;
const int CELL_HEIGHT = 4;

struct Table {
    int table_size;
    int padding_left;
    int padding_top;
    int remained_pairs;
    int *character_list;
    int *character_order;
    Cell **table_data;

    Table(int _table_size, int _TL_x, int _TL_y);
    ~Table();

    int getXInConsole(int _c) const;
    int getYInConsole(int _r) const;

    void generateTableData();
    void displayTableData();
    void printTableData();
};

#endif // TABLE_H
