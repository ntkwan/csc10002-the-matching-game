#ifndef TABLE_H
#define TABLE_H

#include <random>
#include <time.h>
#include <iostream>

#include "cell.h"


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

    generateTableData();
    printTableData();
};

#endif // TABLE_H
