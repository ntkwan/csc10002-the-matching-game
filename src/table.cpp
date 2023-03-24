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
        table_data[x][y].setCellValue((char)(character_list[i] + 'A'));
    }

    delete[] character_list;
    character_list = nullptr;
    delete[] character_order;
    character_order = nullptr;
    delete[] is_marked;
    is_marked = nullptr;
}

int Table::getXInConsole(int c) const { return padding_left + 5 + CELL_LENGTH * c; }

int Table::getYInConsole(int r) const { return padding_top + 2 + CELL_HEIGHT * r; }


void Table::displayTableData() {
    Screen::setConsoleColor(15, 8);
    Screen::clearConsole();

    for (int i = 0; i < table_size; i++) {
        Screen::gotoXY(getXInConsole(i), padding_top - 1);
        putchar(i + '1');
    }

    for (int i = 0; i < table_size; i++) {
        Screen::gotoXY(padding_left - 1, getYInConsole(i));
        putchar(i + 'A');
    }

    Screen::setConsoleColor(15, 0);

    Screen::gotoXY(padding_left + 1, padding_top);
	putchar(201);
	for (int i = 1; i < table_size * CELL_LENGTH; i++) {
		Sleep(2);
		if (i % CELL_LENGTH == 0) {
            putchar(205);
		}
		else {
			putchar(205);
		}
	}
	putchar(187);


	for (int i = 1; i < table_size * CELL_HEIGHT; i++) {
		Sleep(5);
		Screen::gotoXY(table_size * CELL_LENGTH + padding_left + 1, i + padding_top);
		if (i % CELL_HEIGHT == 0) {
            putchar(186);
		}
		else {
			putchar(186);
		}
	}
	Screen::gotoXY(table_size * CELL_LENGTH + padding_left + 1, table_size * CELL_HEIGHT + padding_top);
	putchar(188);

	for (int i = 1; i < table_size * CELL_LENGTH; i++) {
		Screen::gotoXY(table_size * CELL_LENGTH + padding_left - i + 1, table_size * CELL_HEIGHT + padding_top);
		Sleep(2);
		if (i % CELL_LENGTH == 0) {
            putchar(205);
		}
		else {
			putchar(205);
		}
	}
	Screen::gotoXY(padding_left + 1, table_size * CELL_HEIGHT + padding_top);
	putchar(200);

	for (int i = 1; i < table_size * CELL_HEIGHT; i++) {
		Sleep(5);
		Screen::gotoXY(padding_left + 1, table_size * CELL_HEIGHT + padding_top - i);
		if (i % CELL_HEIGHT == 0) {
			putchar(186);
		}
		else {
			putchar(186);
		}
	}

	for (int i = 1; i < table_size * CELL_HEIGHT; i++) {
		for (int j = CELL_LENGTH; j < table_size * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(179);
			}
		}
		Sleep(5);
	}

	for (int i = 1; i < table_size * CELL_LENGTH; i++) {
		for (int j = CELL_HEIGHT; j < table_size * CELL_HEIGHT; j += CELL_HEIGHT) {
			Screen::gotoXY(i + padding_left + 1, j + padding_top);
			if (i % CELL_LENGTH == 0) {
                putchar(32);
			}
			else {
				putchar(196);
			}
		}
		Sleep(2);
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

