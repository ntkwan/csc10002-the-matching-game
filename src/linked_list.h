#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

#include "cell.h"

struct LinkedList {
    Cell* head;
    Cell* tail;
    int list_size;

    LinkedList();

    Cell* getHead();
    Cell* getTail();
    Cell* createNode(Cell*);
    bool isEmpty();
    Cell* getPos(int);

    void addHead(Cell*);
    void addTail(Cell*);
    void removeHead();
    void removeTail();
    void removePos(int);
    void shiftCellToLeft();
    void removeAll();
    void printList();
};
#endif // LINKED_LIST
