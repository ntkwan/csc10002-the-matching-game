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
    occurs = new int[26];

    for (int i = 0; i < table_size * table_size; i += 2) {
        int char_gen = rand() % 26;
        while (occurs[char_gen] > max_distinct_number) char_gen = rand() % 26;

        character_list[i] = character_list[i+1] = rand() % 26;
        ++occurs[char_gen];
    }
    delete[] occurs;
    occurs = nullptr;

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
        table_data[x][y].setCellPosX(x);
        table_data[x][y].setCellPosY(y);
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

void Table::shuffleTableData() {
    srand(time(NULL));
    Cell *shuffle_list = new Cell[table_size * table_size];
    int sz_list = 0;

    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            if (table_data[i][j].getCellState() == FREE) {
                shuffle_list[sz_list] = table_data[i][j];
                ++sz_list;
            }
        }
    }

    int *shuffle_order = new int[sz_list];
    bool *is_marked = new bool[sz_list];
    for (int i = 0; i < sz_list; ++i) {
        is_marked[i] = false;
    }

    for (int i = 0; i < sz_list; ++i) {
        int cellPos = 0;
        cellPos = rand() % sz_list;
        while (is_marked[cellPos] == true) {
            cellPos = rand() % sz_list;
        }
        is_marked[cellPos] = true;
        shuffle_order[i] = cellPos;
    }

    int idx = 0;
    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            if (table_data[i][j].getCellState() == FREE) {
                int order = shuffle_order[idx];
                table_data[i][j] = shuffle_list[order];
                ++idx;
            }
        }
    }

    delete[] shuffle_list;
    shuffle_list = nullptr;
    delete[] shuffle_order;
    shuffle_order = nullptr;
    delete[] is_marked;
    is_marked = nullptr;
}
