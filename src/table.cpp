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

    table_image = new std::string[table_size * 10];
}

Table::~Table() {
    for (int i = 0; i < table_size; ++i) {
        delete[] table_data[i];
    }

    delete[] table_data;
    table_data = nullptr;

    delete[] table_image;
    table_image = nullptr;
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

void Table::cleanMatchingEffect() {
    Screen::setConsoleColor(WHITE, GRAY);

    //left border cleaned
    for (int i = 0; i < table_size; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(padding_left+1, getYInConsole(i));
        putchar(186);

        Screen::setConsoleColor(WHITE, GRAY);
        Screen::gotoXY(padding_left-1, getYInConsole(i));
        putchar(i+'A');

        Screen::gotoXY(padding_left-3, getYInConsole(i)+2);
        putchar(' ');
    }

    //right border cleaned
    for (int i = 0;i < table_size; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(table_size * CELL_LENGTH + padding_left+1, getYInConsole(i));
        putchar(186);

        Screen::gotoXY(table_size * CELL_LENGTH + padding_left+5, getYInConsole(i)+2);
        putchar(' ');
    }
}

void Table::displayTableData() {
    Screen::setConsoleColor(WHITE, GRAY);
    Screen::clearConsole();

    for (int i = 0; i < table_size; ++i) {
        Screen::gotoXY(getXInConsole(i), padding_top-1);
        putchar(i + '1');
    }

    for (int i = 0; i < table_size; ++i) {
        Screen::gotoXY(padding_left-1, getYInConsole(i));
        putchar(i + 'A');
    }

    Screen::setConsoleColor(WHITE, BLACK);

    //top border
    Screen::gotoXY(padding_left + 1, padding_top);
	putchar(201);
	for (int i = 1; i < table_size * CELL_LENGTH; ++i) {
		Sleep(2);
        putchar(205);
	}
	putchar(187);

    //right border
	for (int i = 1; i < table_size * CELL_HEIGHT; ++i) {
		Sleep(5);
		Screen::gotoXY(table_size * CELL_LENGTH + padding_left + 1, i + padding_top);
        putchar(186);
	}
	Screen::gotoXY(table_size * CELL_LENGTH + padding_left + 1, table_size * CELL_HEIGHT + padding_top);
	putchar(188);

    //bottom border
	for (int i = 1; i < table_size * CELL_LENGTH; ++i) {
		Screen::gotoXY(table_size * CELL_LENGTH + padding_left - i + 1, table_size * CELL_HEIGHT + padding_top);
		Sleep(2);
        putchar(205);
	}
	Screen::gotoXY(padding_left + 1, table_size * CELL_HEIGHT + padding_top);
	putchar(200);

    //left border
	for (int i = 1; i < table_size * CELL_HEIGHT; ++i) {
		Sleep(5);
		Screen::gotoXY(padding_left + 1, table_size * CELL_HEIGHT + padding_top - i);
        putchar(186);
	}

	for (int i = 1; i < table_size * CELL_HEIGHT; ++i) {
		for (int j = CELL_LENGTH; j < table_size * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(179);
			}
		}
		Sleep(5);
	}

	for (int i = 1; i < table_size * CELL_LENGTH; ++i) {
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

    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            Screen::gotoXY(table_data[i][j].getCellCoordX(), table_data[i][j].getCellCoordY());
            putchar(table_data[i][j].cell_value);
        }
    }

}

void Table::displayTableBackground() {
    Screen::setConsoleColor(WHITE, WHITE);
	for (int i = 1; i < table_size * CELL_HEIGHT; ++i) {
		for (int j = CELL_LENGTH; j < table_size * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(' ');
			}
		}
	}

	for (int i = 1; i < table_size * CELL_LENGTH; ++i) {
		for (int j = CELL_HEIGHT; j < table_size * CELL_HEIGHT; j += CELL_HEIGHT) {
			Screen::gotoXY(i + padding_left + 1, j + padding_top);
			if (i % CELL_LENGTH == 0) {
                putchar(' ');
			}
			else {
				putchar(' ');
			}
		}
	}
}

void Table::loadTableBackground(const std::string &path) {
    std::fstream bg(path, std::fstream::in);

    for (int idx = 0; !bg.eof(); ++idx) {
        getline(bg, table_image[idx]);
    }

    bg.close();
}

void Table::printTableData() {
    for (int i = 0; i < table_size; ++i) {
        for (int j = 0; j < table_size; ++j) {
            std::cout<<table_data[i][j].cell_value<<" ";
        }
        std::cout<<"\n";
    }
}

