#include "table.h"

Table::Table(int _table_size, int _padding_left, int _padding_top) {
    table_size = _table_size;
    padding_left = _padding_left;
    padding_top = _padding_top;

    remained_pairs = (table_size * table_size) / 2;

    table_data = new Cell*[table_size];
    for (int i = 0; i < table_size; ++i) {
        table_data[i] = new Cell[table_size];
    }
}

Table::~Table() {
    for (int i = 0; i < table_size; ++i) {
        delete[] table_data[i];
    }

    delete[] table_data;
    table_data = nullptr;
}

int Table::getXInConsole(int _c) const { return padding_left + 5 + CELL_LENGTH * _c; }

int Table::getYInConsole(int _r) const { return padding_top + 2 + CELL_HEIGHT * _r; }

void Table::generateTableData() {
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
        int x = character_order[i] / table_size;
        int y = character_order[i] % table_size;
        table_data[x][y].setCellCoordX(getXInConsole(x));
        table_data[x][y].setCellCoordY(getYInConsole(y));
        table_data[x][y].setCellValue((char)(character_list[i] + 'A'));
    }

    delete[] character_list;
    character_list = nullptr;
    delete[] character_order;
    character_order = nullptr;
    delete[] is_marked;
    is_marked = nullptr;
}

void Table::displayTableData() {
    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            Screen::gotoXY(table_data[i][j].getCellCoordX(), table_data[i][j].getCellCoordY());
            putchar(table_data[i][j].cell_value);
        }
    }
}

void Table::printTableData() {
    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            std::cout<<table_data[i][j].cell_value<<" ";
        }
        std::cout<<"\n";
    }
}

