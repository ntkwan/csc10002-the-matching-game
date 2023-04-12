#include "table.h"

Table::Table() {}

Table::Table(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top) {
    table_size_n = _table_size_n;
    table_size_m = _table_size_m;
    padding_left = _padding_left;
    padding_top = _padding_top;

    remained_pairs = (table_size_n * table_size_m) / 2;

    table_data = new Cell*[table_size_n];
    for (int i = 0; i < table_size_n; ++i) {
        table_data[i] = new Cell[table_size_m];
    }
}

Table::~Table() {
    for (int i = 0; i < table_size_n; ++i) {
        delete[] table_data[i];
    }

    delete[] table_data;
    table_data = nullptr;
}

int Table::getXInConsole(int _c) const { return padding_left + 5 + CELL_LENGTH * _c; }

int Table::getYInConsole(int _r) const { return padding_top + 2 + CELL_HEIGHT * _r; }

void Table::generateTableData() {
    srand(time(NULL));

    character_list = new int[table_size_n * table_size_m];
    occurs = new int[26];
    for (int i = 0; i < 26; ++i) {
        occurs[i] = 0;
    }

    for (int i = 0; i < table_size_n * table_size_m; i += 2) {
        int char_gen = rand() % 26;
        while (occurs[char_gen] > max_distinct_number) char_gen = rand() % 26;

        character_list[i] = character_list[i+1] = char_gen;
        ++occurs[char_gen];
    }


    delete[] occurs;
    occurs = nullptr;

    character_order = new int[table_size_n * table_size_m];
    bool *is_marked = new bool[table_size_n * table_size_m];
    for (int i = 0; i < table_size_n * table_size_m; ++i) {
        is_marked[i] = false;
    }

    for (int i = 0; i < table_size_n * table_size_m; ++i) {
        int cell_pos = 0;
        cell_pos = rand() % (table_size_n * table_size_m);
        while (is_marked[cell_pos] == true) {
            cell_pos = rand() % (table_size_n * table_size_m);
        }
        is_marked[cell_pos] = true;
        character_order[i] = cell_pos;
    }

    int idx = 0;
    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            int order = character_order[idx];
            table_data[i][j].setCellPosX(i);
            table_data[i][j].setCellPosY(j);
            table_data[i][j].setCellCoordX(getXInConsole(i));
            table_data[i][j].setCellCoordY(getYInConsole(j));
            table_data[i][j].setCellValue((char)(character_list[order] + 'A'));
            ++idx;
        }
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
    Cell *shuffle_list = new Cell[table_size_n * table_size_m];
    int sz_list = 0;

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            int cell_state = table_data[i][j].getCellState();
            if (cell_state == LOCKED) {
                table_data[i][j].setCellState(FREE);
            }
            if (cell_state == FREE) {
                shuffle_list[sz_list] = table_data[i][j];
                ++sz_list;
            }
        }
    }

    occurs = new int[26];
    for (int i = 0; i < 26; ++i) {
        occurs[i] = 0;
    }

    for (int i = 0; i < sz_list; i += 2) {
        int char_gen = rand() % 26;
        while (occurs[char_gen] > max_distinct_number) char_gen = rand() % 26;

        shuffle_list[i].cell_value = shuffle_list[i+1].cell_value = (char)(char_gen + 'A');
        ++occurs[char_gen];
    }
    delete[] occurs;
    occurs = nullptr;

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

    for (int i = 0; i < sz_list; ++i) {
        int order = shuffle_order[i];
        int x = shuffle_list[i].cell_pos_x;
        int y = shuffle_list[i].cell_pos_y;
        table_data[x][y].setCellPosX(x);
        table_data[x][y].setCellPosY(y);
        table_data[x][y].setCellCoordX(getXInConsole(x));
        table_data[x][y].setCellCoordY(getYInConsole(y));
        table_data[x][y].setCellValue(shuffle_list[order].cell_value);
        table_data[x][y].setCellState(shuffle_list[order].cell_state);
    }

    delete[] shuffle_list;
    shuffle_list = nullptr;
    delete[] shuffle_order;
    shuffle_order = nullptr;
    delete[] is_marked;
    is_marked = nullptr;
}
