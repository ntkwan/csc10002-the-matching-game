#include "game_scene.h"

GameScene::GameScene(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top) {
    table_size_n = _table_size_n;
    table_size_m = _table_size_m;
    padding_left = _padding_left;
    padding_top = _padding_top;

    table_image = new std::string[100];
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
    for (int i = 0; i < table_size_n; ++i) {
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
    for (int i = 0; i < table_size_m; ++i) {
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
    for (int i = 0; i < table_size_m; ++i) {
        Screen::setConsoleColor(WHITE, BLACK);
        Screen::gotoXY(table_size_n * CELL_LENGTH+padding_left + 1, getYInConsole(i));
        putchar(186);

        Screen::gotoXY(table_size_n * CELL_LENGTH+padding_left + 5, getYInConsole(i) + 2);
        putchar(' ');
    }

    //bottom border cleaned
    for (int i = 0; i < table_size_n; ++i) {
        Screen::gotoXY(getXInConsole(i) + 4, table_size_m * CELL_HEIGHT + padding_top + 2);
        putchar(' ');

        Screen::gotoXY(getXInConsole(i), table_size_m * CELL_HEIGHT + padding_top);
        putchar(205);
    }
}

void GameScene::displayInfomationBoard(int left, int top, int width, int height) {
	Screen::gotoXY(left, top);
	putchar(218);
	for (int i = 0; i < width; ++i) putchar(196);
	putchar(191);

	for (int i = 0; i < height; ++i)
	{
		Screen::gotoXY(left, top + i + 1);
		putchar(179);
		Screen::gotoXY(left + width + 1, top + i + 1);
		putchar(179);
		if (i == height-1) {
            Screen::gotoXY(left, top + i + 1);
            putchar(192);
		}
	}

	for (int i = 0; i < width; ++i) putchar(196);
	putchar(217);
}

void GameScene::displayUserInterface(int _padding_left, int _padding_top, int _mode) {
    displayInfomationBoard(_padding_left, 1, 50, 10);
    displayInfomationBoard(_padding_left, 11, 50, 10);
    displayInfomationBoard(_padding_left, 21, 50, 11);

    auto printInf = [](const std::string& text, const int pd_left, const int pd_top) {
        Screen::gotoXY(pd_left, pd_top);
        std::cout<<text;
    };

    Screen::setConsoleColor(WHITE, RED);
    printInf("PLAYER INFORMATION", _padding_left + 18, 1);
    printInf("GAME NOTIFICATION", _padding_left + 18, 11);
    printInf("QUICK INSTRUCTIONS", _padding_left + 18, 21);

    Screen::setConsoleColor(WHITE, BLACK);
    printInf("USERNAME: ", _padding_left + 5, 3);
    printInf("CURRENT POINTS: ", _padding_left + 5, 5);
    printInf("BEST POINTS: ", _padding_left + 5, 6);
    printInf("LEVELS PLAYED: ", _padding_left + 5, 8);
    printInf("HIGHEST LEVEL: ", _padding_left + 5, 9);
    printInf("MISTAKES REMAIN: ", _padding_left + 5, 13);

    auto printIns = [](const std::string& text_1, const std::string &text_2, const int pd_left, const int pd_top) {
        Screen::gotoXY(pd_left, pd_top);
        Screen::setConsoleColor(WHITE, YELLOW);
        std::cout<<text_1<<": ";
        Screen::setConsoleColor(WHITE, BLACK);
        std::cout<<text_2;
    };

    printIns("ARROWS", "MOVE", _padding_left + 5, 23);
    printIns("ENTER", "SELECT CELL", _padding_left + 5, 25);
    printIns("H", "MOVING SUGGESTIONS", _padding_left + 5, 27);
    printIns("F", "SHUFFLE", _padding_left + 5, 29);
    printIns("ESC", "EXIT THE GAME", _padding_left + 5, 31);

    if (_mode == STANDARD_MODE) {
        std::string *flowers = new std::string[20];
        auto loadFlowersAssets = [](const std::string &path, std::string *&flowers) {
            std::fstream in(path, std::fstream::in);
            int n = 0;
            for (int i = 0; !in.eof(); ++i, n = i) {
                getline(in, flowers[i]);
            }
            in.close();
            return n;
        };

        auto displayFlowersAssets = [](const std::string *flowers, const int n, const int pd_left, const int pd_top) {
            for (int i = 0; i < n; ++i) {
                Screen::gotoXY(pd_left, pd_top + i);
                std::cout<<flowers[i];
            }
        };

        int n = loadFlowersAssets("assets/left_flowers.txt", flowers);
        _padding_left = 0;
        _padding_top = 28;
        Screen::setConsoleColor(WHITE, GREEN);
        displayFlowersAssets(flowers, n, _padding_left, _padding_top);

        n = loadFlowersAssets("assets/right_flowers.txt", flowers);
        displayFlowersAssets(flowers, n, _padding_left + 122, _padding_top - 1);
    }

    Screen::setConsoleColor(WHITE, BLACK);
}

void GameScene::displayNotification(int _padding_left, int _padding_top, const std::string &text, int delayed_time) {
    Screen::setConsoleColor(WHITE, GREEN);
    Screen::gotoXY(_padding_left, _padding_top);
    std::cout<<text;
    Sleep(delayed_time);
    Screen::gotoXY(_padding_left, _padding_top);
    for (size_t i = 0; i < text.size(); ++i) std::cout<<" ";
}

void GameScene::displayTableBorder() {
    Screen::setConsoleColor(WHITE, GRAY);
    Screen::clearConsole();

    for (int i = 0; i < table_size_n; ++i) {
        Screen::gotoXY(getXInConsole(i), padding_top-1);
        putchar(i + '1');
    }

    for (int i = 0; i < table_size_m; ++i) {
        Screen::gotoXY(padding_left-1, getYInConsole(i));
        putchar(i + 'A');
    }

    Screen::setConsoleColor(WHITE, BLACK);

    //top border
    Screen::gotoXY(padding_left + 1, padding_top);
	putchar(201);
	for (int i = 1; i < table_size_n * CELL_LENGTH; ++i) {
		Sleep(2);
        putchar(205);
	}
	putchar(187);

    //right border
	for (int i = 1; i < table_size_m * CELL_HEIGHT; ++i) {
		Sleep(5);
		Screen::gotoXY(table_size_n * CELL_LENGTH + padding_left + 1, i + padding_top);
        putchar(186);
	}
	Screen::gotoXY(table_size_n * CELL_LENGTH + padding_left + 1, table_size_m * CELL_HEIGHT + padding_top);
	putchar(188);

    //bottom border
	for (int i = 1; i < table_size_n * CELL_LENGTH; ++i) {
		Screen::gotoXY(table_size_n * CELL_LENGTH + padding_left - i + 1, table_size_m * CELL_HEIGHT + padding_top);
		Sleep(2);
        putchar(205);
	}
	Screen::gotoXY(padding_left + 1, table_size_m * CELL_HEIGHT + padding_top);
	putchar(200);

    //left border
	for (int i = 1; i < table_size_m * CELL_HEIGHT; ++i) {
		Sleep(5);
		Screen::gotoXY(padding_left + 1, table_size_m * CELL_HEIGHT + padding_top - i);
        putchar(186);
	}

    //horizontal line inside the table
	for (int i = 1; i < table_size_m * CELL_HEIGHT; ++i) {
		for (int j = CELL_LENGTH; j < table_size_n * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(179);
			}
		}
		Sleep(5);
	}

	//vertical line inside the table
	for (int i = 1; i < table_size_n * CELL_LENGTH; ++i) {
		for (int j = CELL_HEIGHT; j < table_size_m * CELL_HEIGHT; j += CELL_HEIGHT) {
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
	for (int i = 1; i < table_size_m * CELL_HEIGHT; ++i) {
		for (int j = CELL_LENGTH; j < table_size_n * CELL_LENGTH; j += CELL_LENGTH) {
			if (i % CELL_HEIGHT != 0) {
				Screen::gotoXY(j + padding_left + 1, i + padding_top);
				putchar(' ');
			}
		}
	}

	for (int i = 1; i < table_size_n * CELL_LENGTH; ++i) {
		for (int j = CELL_HEIGHT; j < table_size_m * CELL_HEIGHT; j += CELL_HEIGHT) {
			Screen::gotoXY(i + padding_left + 1, j + padding_top);
            putchar(' ');
		}
	}
}

