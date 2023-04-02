#include "linked_list_table.h"

TableLL::TableLL(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top) {
    table_size_n = _table_size_n;
    table_size_m = _table_size_m;
    padding_left = _padding_left;
    padding_top = _padding_top;

    remained_pairs = (table_size_n * table_size_m) / 2;

    table_data = new LinkedList[table_size_m];
}

TableLL::~TableLL() {
    for (int i = 0; i < table_size_m; ++i) {
        table_data[i].removeAll();
    }

    delete[] table_data;
    table_data = nullptr;
}

int TableLL::getXInConsole(int _c) const { return padding_left + 5 + CELL_LENGTH * _c; }

int TableLL::getYInConsole(int _r) const { return padding_top + 2 + CELL_HEIGHT * _r; }

void TableLL::generateTableData() {
    srand(time(NULL));

    character_list = new int[table_size_n * table_size_m];
    occurs = new int[26];
    for (int i = 0; i < 26; ++i) {
        occurs[i] = 0;
    }

    for (int i = 0; i < table_size_n * table_size_m; i += 2) {
        int char_gen = rand() % 26;
        while (occurs[char_gen] > max_distinct_number) char_gen = rand() % 26;

        character_list[i] = character_list[i+1] = rand() % 26;
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

    for (int i = 0; i < table_size_n * table_size_m; ++i) {
        int y = character_order[i] % table_size_m;
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

void TableLL::shuffleTableData() {
    srand(time(NULL));
    Cell *shuffle_list = new Cell[table_size_n * table_size_m];
    int sz_list = 0;

    for (int i = 0; i < table_size_m; ++i) {
        for (Cell* cur_node = table_data[i].head; cur_node != nullptr; cur_node = cur_node->next) {
            if (cur_node->cell_state == LOCKED) {
                cur_node->cell_state = FREE;
            }
            if (cur_node->cell_state == FREE) {
                shuffle_list[sz_list] = cur_node;
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

        shuffle_list[i].cell_value = shuffle_list[i+1].cell_value = (char)(rand() % 26 + 'A');
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
        int cell_pos = 0;
        cell_pos = rand() % sz_list;
        while (is_marked[cell_pos] == true) {
            cell_pos = rand() % sz_list;
        }
        is_marked[cell_pos] = true;
        shuffle_order[i] = cell_pos;
    }

    int idx = 0;
    for (int i = 0; i < table_size_m; ++i) {
        for (Cell* cur_node = table_data[i].head; cur_node != nullptr; cur_node = cur_node->next) {
            if (cur_node->cell_state == FREE) {
                int order = shuffle_order[idx];
                int x = shuffle_list[idx].cell_pos_x;
                int y = shuffle_list[idx].cell_pos_y;
                int cell_state = shuffle_list[order].cell_state;
                int cell_value = shuffle_list[order].cell_value;
                cur_node->setCellPosX(x);
                cur_node->setCellPosY(y);
                cur_node->setCellCoordX(getXInConsole(x));
                cur_node->setCellCoordY(getYInConsole(y));
                cur_node->setCellState(cell_state);
                cur_node->setCellValue(cell_value);
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


