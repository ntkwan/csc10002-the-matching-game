#ifndef TABLE_H
#define TABLE_H

#include <random>
#include <time.h>
#include <iostream>

#include "screen.h"
#include "cell.h"

struct Table {
    int table_size_n; //table row size
    int table_size_m; //table column size
    int padding_left; //table's left padding
    int padding_top; //table's top padding
    int remained_pairs; //table pairs reminaning
    int *character_list; //array to hold random characters
    int *character_order;  //array to hold random positions
    int max_distinct_number = 1; //handle character's unique
    int *occurs; //array to handle character occurences
    Cell **table_data; //2D pointer array

    Table(); //table constructor
    Table(int, int, int, int); //init table structure
    ~Table(); //table destructor

    int getXInConsole(int) const; //get position x on the console
    int getYInConsole(int) const; //get position y on the console

    void generateTableData(); //generate table data
    void shuffleTableData(); //table shuffling feature - shuffle table data while in-game
    void printTableData(); //print table data to the console application (for debugging)
};

#endif // TABLE_H
