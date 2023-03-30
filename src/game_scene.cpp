#include "game_scene.h"

GameScene::GameScene(int _table_size, int _padding_left, int _padding_top) {
    table_size = _table_size;
    padding_left = _padding_left;
    padding_top = _padding_top;

    table_image = new std::string[table_size * 10];
}

GameScene::~GameScene() {
    delete[] table_image;
    table_image = nullptr;
}

int GameScene::getXInConsole(int _c) { return padding_left + 5 + CELL_LENGTH * _c; }

int GameScene::getYInConsole(int _r) { return padding_top + 2 + CELL_HEIGHT * _r; }

void GameScene::cleanMatchingEffect() {
    Screen::setConsoleColor(WHITE, GRAY);

    //top border cleaned
    for (int i = 0; i < table_size; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(getXInConsole(i), padding_top);
        putchar(205);

        Screen::setConsoleColor(WHITE, GRAY);
        Screen::gotoXY(getXInConsole(i), padding_top - 1);
        putchar(i + '1');

        Screen::gotoXY(getXInConsole(i) + 4, padding_top - 2);
        putchar(' ');
    }

    //left border cleaned
    for (int i = 0; i < table_size; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(padding_left + 1, getYInConsole(i));
        putchar(186);

        Screen::setConsoleColor(WHITE, GRAY);
        Screen::gotoXY(padding_left - 1, getYInConsole(i));
        putchar(i + 'A');

        Screen::gotoXY(padding_left - 3, getYInConsole(i) + 2);
        putchar(' ');
    }

    //right border cleaned
    for (int i = 0; i < table_size; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(table_size * CELL_LENGTH+padding_left + 1, getYInConsole(i));
        putchar(186);

        Screen::gotoXY(table_size * CELL_LENGTH+padding_left + 5, getYInConsole(i) + 2);
        putchar(' ');
    }

    //bottom border cleaned
    for (int i = 0; i < table_size; ++i) {
        Screen::gotoXY(getXInConsole(i) + 4, table_size * CELL_HEIGHT + padding_top+2);
        putchar(' ');

        Screen::gotoXY(getXInConsole(i), table_size * CELL_HEIGHT + padding_top);
        putchar(205);
    }
}

void GameScene::displayTableBorder() {
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

    //horizontal line inside the table
	for (int i = 1; i < table_size * CELL_HEIGHT; ++i) {
		for (int j = CELL_LENGTH; j < table_size * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(179);
			}
		}
		Sleep(5);
	}

	//vertical line inside the table
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
}

void GameScene::loadTableBackground(const std::string &path) {
    std::fstream bg(path, std::fstream::in);

    for (int idx = 0; !bg.eof(); ++idx) {
        getline(bg, table_image[idx]);
    }

    bg.close();
}

void GameScene::displayTableBackground() {
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

