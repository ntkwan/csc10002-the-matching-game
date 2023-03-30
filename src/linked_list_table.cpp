#include "linked_list_table.h"

TableLL::TableLL(int _table_size, int _padding_left, int _padding_top) {
    table_size = _table_size;
    padding_left = _padding_left;
    padding_top = _padding_top;

    remained_pairs = (table_size * table_size) / 2;

    table_data = new LinkedList[table_size];
    table_image = new std::string[table_size * 10];
}

TableLL::~TableLL() {
    delete[] table_data;
    table_data = nullptr;

    delete[] table_image;
    table_image = nullptr;
}

int TableLL::getXInConsole(int _c) const { return padding_left + 5 + CELL_LENGTH * _c; }

int TableLL::getYInConsole(int _r) const { return padding_top + 2 + CELL_HEIGHT * _r; }

void TableLL::generateTableData() {
    srand(time(NULL));

    character_list = new int[table_size * table_size];
    for (int i = 0; i < table_size * table_size; i += 2) {
        character_list[i] = character_list[i+1] = rand() % 26;
    }

    character_order = new int[table_size * table_size];
    bool *is_marked = new bool[table_size * table_size];
    for (int i = 0; i < table_size * table_size; ++i) {
        is_marked[i] = false;
    }

    for (int i = 0; i < table_size * table_size; ++i) {
        int cellPos = 0;
        cellPos = rand() % (table_size * table_size);
        while (is_marked[cellPos] == true) {
            cellPos = rand() % (table_size * table_size);
        }
        is_marked[cellPos] = true;
        character_order[i] = cellPos;
    }

    for (int i = 0; i < table_size * table_size; ++i) {
        int y = character_order[i] % table_size;
        int x = table_data[y].list_size;

        Cell* cur_node = new Cell((char)(character_list[i] + 'A'), FREE, getXInConsole(x), getYInConsole(y), x, y);
        table_data[y].addTail(cur_node);
    }

    delete[] character_list;
    character_list = nullptr;
    delete[] character_order;
    character_order = nullptr;
    delete[] is_marked;
    is_marked = nullptr;
}

void TableLL::displayTableData() {
    for (int i = 0; i < table_size; ++i) {
        for (Cell* cur_node = table_data[i].head; cur_node != nullptr; cur_node = cur_node->next) {
            int x = cur_node->getCellCoordX();
            int y = cur_node->getCellCoordY();
            Screen::gotoXY(x, y);
            putchar(cur_node->getCellValue());
        }
    }
}

void TableLL::printTableData() {
    for (int i = 0; i < table_size; ++i) {
        table_data[i].printList();
        std::cout<<"\n";
    }
}


