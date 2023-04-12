#ifndef TABLE_LL_H
#define TABLE_LL_H

#include <random>
#include <time.h>
#include <iostream>

#include "screen.h"
#include "linked_list.h"

struct TableLL {
    int table_size_n;  //table row size
    int table_size_m;  //table column size
    int padding_left; //table's left padding
    int padding_top;  //table's top padding
    int remained_pairs; //table pairs reminaning
    int *character_list; //array to hold random characters
    int *character_order; //array to hold random positions
    int max_distinct_number = 2; //handle character's unique
    int *occurs; //array to handle character occurences
    LinkedList *table_data; //2D linked list

    TableLL(); //table constructor
    TableLL(int, int, int, int); //init table structure
    ~TableLL(); //table destructor

    int getXInConsole(int) const; //get position x on the console
    int getYInConsole(int) const; //get position y on the console

    void generateTableData(); //generate table data
    void shuffleTableData(); //table shuffling feature - shuffle table data while in-game

};
#endif // TABLE_LL_H
