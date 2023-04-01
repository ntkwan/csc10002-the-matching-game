#include "difficult_mode.h"

DifficultMode::DifficultMode(int _mode, int _padding_left, int _padding_top) {
    mode = _mode;
    padding_left = _padding_left;
    padding_top = _padding_top;

    TableObject = new TableLL(mode, padding_left, padding_top);
    GameObject = new GameScene(mode, padding_left, padding_top);

    table_size = TableObject->table_size;
    remained_pairs = table_size * table_size;
    cell_pos_x = 0;
    cell_pos_y = 0;
    cell_coord_x = TableObject->getXInConsole(cell_pos_x);
    cell_coord_y = TableObject->getYInConsole(cell_pos_y);
    end_loop = false;
}

DifficultMode::~DifficultMode() {
    delete TableObject;
    TableObject = nullptr;

    delete GameObject;
    GameObject = nullptr;
}

int DifficultMode::getCellState(int _cell_pos_x, int _cell_pos_y) const { return TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x)->getCellState(); }

char DifficultMode::getCellValue(int _cell_pos_x, int _cell_pos_y) const { return TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x)->getCellValue(); }

void DifficultMode::setCellState(int _cell_pos_x, int _cell_pos_y, int _state)  { TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x)->setCellState(_state); }

void DifficultMode::setCellValue(int _cell_pos_x, int _cell_pos_y, char _value) { TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x)->setCellValue(_value); }

char DifficultMode::getCharAt(int _cell_pos_x, int _cell_pos_y) const {
	if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED) return ' ';
return getCellValue(_cell_pos_x, _cell_pos_y);
}

void DifficultMode::swapPoints(int &first_cell, int &second_cell) {
    int tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
}

void DifficultMode::swapCells(std::pair<int, int> &first_cell, std::pair<int, int> &second_cell) {
    std::pair<int, int> tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
}

void DifficultMode::displayCellBackgroundAt(int _cell_pos_x, int _cell_pos_y, int _background, int _text) {
    Screen::setConsoleColor(_background, _text);
    int _cell_coord_x = TableObject->getXInConsole(_cell_pos_x);
    int _cell_coord_y = TableObject->getYInConsole(_cell_pos_y);
    Screen::gotoXY(_cell_coord_x, _cell_coord_y);

    for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            putchar(GameObject->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
        }
    }
}

void DifficultMode::displayCellValueAt(int _cell_pos_x, int _cell_pos_y, int _background, int _text) {
    Screen::setConsoleColor(_background, _text);
    int _cell_coord_x = TableObject->getXInConsole(_cell_pos_x);
    int _cell_coord_y = TableObject->getYInConsole(_cell_pos_y);
    Screen::gotoXY(_cell_coord_x, _cell_coord_y);

    for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED || getCellState(_cell_pos_x, _cell_pos_y) == EMPTY_BOARD) {
                Screen::setConsoleColor(WHITE, RED);
                putchar(GameObject->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
                Screen::setConsoleColor(_background, _text);
                continue;
            }

            if (current_coord_x == _cell_coord_x && current_coord_y == _cell_coord_y) {
                putchar(getCharAt(_cell_pos_x, _cell_pos_y));
            } else {
                putchar(' ');
            }
        }
    }
}


void DifficultMode::cleanTableDataAtRow(int _cell_pos_y) {
    for (int _cell_pos_x = 0; _cell_pos_x < table_size; ++_cell_pos_x) {
        displayCellBackgroundAt(_cell_pos_x, _cell_pos_y, WHITE, RED);
    }
}

void DifficultMode::displayTableDataAtRow(int _cell_pos_y) {
    for (int _cell_pos_x = 0; _cell_pos_x < table_size; ++_cell_pos_x) {
        displayCellValueAt(_cell_pos_x, _cell_pos_y, WHITE, BLACK);
    }
}

void DifficultMode::deleteCell() {
    if (checkMatching(locked_list[0], locked_list[1], true) == false) {
        reverse(locked_list.begin(), locked_list.end());
        for (auto cell : locked_list) {
            Screen::gotoXY(TableObject->getXInConsole(cell.first), TableObject->getYInConsole(cell.second));
            setCellState(cell.first, cell.second, FREE);
            selectCell(cell.first, cell.second, RED);
        }

        Sleep(500);

        for (auto cell : locked_list) {
            Screen::gotoXY(TableObject->getXInConsole(cell.first), TableObject->getYInConsole(cell.second));
            selectCell(cell.first, cell.second, WHITE);
        }

        locked_list.clear();
    } else {
        remained_pairs -= 2;

        if (locked_list[0].second == locked_list[1].second) {

            std::pair<int, int> first_cell = locked_list[0];
            std::pair<int, int> second_cell = locked_list[1];

            if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

            cleanTableDataAtRow(first_cell.second);

            for (int i = 2; i >= 1; --i) {
                if (i == 2) TableObject->table_data[first_cell.second].removePos(first_cell.first);
                else TableObject->table_data[first_cell.second].removePos(second_cell.first-1);

                int x = table_size-1;
                int y = first_cell.second;
                Cell* dummy_node = new Cell(' ', DELETED, TableObject->getXInConsole(x), TableObject->getYInConsole(y), x, y);

                TableObject->table_data[first_cell.second].addTail(dummy_node);
            }
        } else {

            for (auto cell : locked_list) {
                cleanTableDataAtRow(cell.second);
                TableObject->table_data[cell.second].removePos(cell.first);

                int x = table_size-1;
                int y = cell.second;
                Cell* dummy_node = new Cell(' ', DELETED, TableObject->getXInConsole(x), TableObject->getYInConsole(y), x, y);

                TableObject->table_data[cell.second].addTail(dummy_node);
            }
        }


        Screen::setConsoleColor(WHITE, BLACK);
        for (auto cell : locked_list) {
            displayTableDataAtRow(cell.second);
        }

        locked_list.clear();
    }
}

void DifficultMode::selectCell(int _cell_pos_x, int _cell_pos_y, int _background) {
    int _cell_coord_x = TableObject->getXInConsole(_cell_pos_x);
    int _cell_coord_y = TableObject->getYInConsole(_cell_pos_y);
    Screen::gotoXY(_cell_coord_x, _cell_coord_y);

    if (getCellState(cell_pos_x, cell_pos_y) == DELETED) {
        Screen::setConsoleColor(GREEN, RED);
    } else {
        if (getCellState(cell_pos_x, cell_pos_y) == LOCKED || getCellState(cell_pos_x, cell_pos_y) == EMPTY_BOARD) {
            Screen::setConsoleColor(_background, RED);
        } else {
            Screen::setConsoleColor(_background, BLACK);
        }
    }

    for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED || getCellState(_cell_pos_x, _cell_pos_y) == EMPTY_BOARD) {
                putchar(GameObject->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
                continue;
            }

            if (current_coord_x == _cell_coord_x && current_coord_y == _cell_coord_y) {
                putchar(getCharAt(_cell_pos_x, _cell_pos_y));
            }
            else {
                putchar(' ');
            }
        }
    }

    Screen::gotoXY(_cell_coord_x, _cell_coord_y);
}

void DifficultMode::unselectCell() {
    cell_coord_x = TableObject->getXInConsole(cell_pos_x);
    cell_coord_y = TableObject->getYInConsole(cell_pos_y);
    Screen::gotoXY(cell_coord_x, cell_coord_y);

    if (getCellState(cell_pos_x, cell_pos_y) == LOCKED) {
        Screen::setConsoleColor(YELLOW, BLACK);
    } else {
        if (getCellState(cell_pos_x, cell_pos_y) == DELETED) {
            Screen::setConsoleColor(WHITE, RED);
        } else {
            Screen::setConsoleColor(WHITE, BLACK);
        }
    }

    for (int current_coord_y = cell_coord_y - 1; current_coord_y <= cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = cell_coord_x - 3; current_coord_x <= cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (getCellState(cell_pos_x, cell_pos_y) == DELETED) {
                putchar(GameObject->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
                continue;
            }

            if (current_coord_x == cell_coord_x && current_coord_y == cell_coord_y) {
                putchar(getCharAt(cell_pos_x, cell_pos_y));
            }
            else {
                putchar(' ');
            }
        }
    }

    Screen::gotoXY(cell_coord_x, cell_coord_y);
}

void DifficultMode::lockCell() {
    int cell_state = getCellState(cell_pos_x, cell_pos_y);

    if (cell_state == LOCKED || cell_state == DELETED) return;

    selectCell(cell_pos_x, cell_pos_y, YELLOW);
    setCellState(cell_pos_x, cell_pos_y, LOCKED);
    locked_list.push_back(std::pair<int, int>(cell_pos_x, cell_pos_y));

    if (locked_list.size() == 2) {
        Sleep(100);
        deleteCell();
        Sleep(100);
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::moveUp() {
    if (cell_pos_y > 0) {
        unselectCell();
        --cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::moveDown() {
    if (cell_pos_y < table_size-1) {
        unselectCell();
        ++cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::moveLeft() {
    if (cell_pos_x > 0) {
        unselectCell();
        --cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::moveRight() {
    if (cell_pos_x < table_size-1) {
        unselectCell();
        ++cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::initTable() {
    TableObject->generateTableData();
    GameObject->displayTableBorder();
    TableObject->displayTableData();
    GameObject->loadTableBackground("assets/charmander.txt");
    displayUserInterface();
}

void DifficultMode::displayUserInterface() {
    int _padding_left = 80;
    int _padding_top = 0;
    GameObject->displayInfomationBoard(_padding_left, 1, 50, 10);
    GameObject->displayInfomationBoard(_padding_left, 11, 50, 10);
    GameObject->displayInfomationBoard(_padding_left, 21, 50, 11);

    Screen::setConsoleColor(WHITE, RED);
    Screen::gotoXY(_padding_left + 18, 1);
    std::cout<<"PLAYER INFORMATION";
    Screen::gotoXY(_padding_left + 18, 11);
    std::cout<<"GAME NOTIFICATION";
    Screen::gotoXY(_padding_left + 18, 21);
    std::cout<<"QUICK INSTRUCTIONS";

    Screen::setConsoleColor(WHITE, BLACK);
    Screen::gotoXY(_padding_left + 5, 3);
    std::cout<<"USERNAME: ";
    Screen::gotoXY(_padding_left + 5, 5);
    std::cout<<"CURRENT POINTS: ";
    Screen::gotoXY(_padding_left + 5, 6);
    std::cout<<"BEST POINTS: ";
    Screen::gotoXY(_padding_left + 5, 8);
    std::cout<<"LEVELS PLAYED: ";
    Screen::gotoXY(_padding_left + 5, 9);
    std::cout<<"HIGHEST LEVEL: ";

    Screen::gotoXY(_padding_left + 5, 13);
    std::cout<<"MISTAKES REMAIN: ";

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

    Screen::setConsoleColor(WHITE, BLACK);
}

void DifficultMode::startGame() {
    Screen::clearConsole();
    initTable();

    selectCell(cell_pos_x, cell_pos_y, GREEN);

    while (end_loop == false && remained_pairs > 0) {
        switch(Screen::getConsoleInput()) {
            case 1:
                    end_loop = true;
                    break;
            case 2:
                    moveUp();
                    break;
            case 3:
                    moveLeft();
                    break;
            case 4:
                    moveRight();
                    break;
            case 5:
                    moveDown();
                    break;
            case 6:
                    lockCell();
                    break;
            case 7:
                    findValidPairs();
                    break;
        }
    }

    setCellState(cell_pos_x, cell_pos_y, EMPTY_BOARD);
    selectCell(cell_pos_x, cell_pos_y, WHITE);
    GameObject->displayTableBackground();
    Screen::setConsoleColor(WHITE, BLACK);
}


bool DifficultMode::isCharacterEqual(std::pair<int,int> first_cell, std::pair<int, int> second_cell) { return (getCellValue(first_cell.first, first_cell.second) == getCellValue(second_cell.first, second_cell.second)); }

bool DifficultMode::checkIMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
	if (first_cell.first == second_cell.first) {
		int start_point = first_cell.second;
		int end_point = second_cell.second;
		if (start_point > end_point) swapPoints(start_point, end_point);

		for (int current_pos_y = start_point+1; current_pos_y < end_point; ++current_pos_y) {
			if (getCellState(first_cell.first, current_pos_y) != DELETED) return false;
		}

		return true;
	}

	if (first_cell.second == second_cell.second) {
		int start_point = first_cell.first;
		int end_point = second_cell.first;
		if (start_point > end_point) swapPoints(start_point, end_point);

		for (int current_pos_x = start_point+1; current_pos_x < end_point; ++current_pos_x) {
			if (getCellState(current_pos_x, first_cell.second) != DELETED) return false;
		}

		return true;
	}

return false;
}

void DifficultMode::displayILine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
	if (first_cell.first == second_cell.first) {
		int start_point = first_cell.second;
		int end_point = second_cell.second;
		if (start_point > end_point) swapPoints(start_point, end_point);

        int x = TableObject->getXInConsole(first_cell.first);
        start_point = TableObject->getYInConsole(start_point);
        end_point = TableObject->getYInConsole(end_point);

        if (overwrite == false) {
            Screen::setConsoleColor(YELLOW, RED);
        } else {
            Screen::setConsoleColor(WHITE, BLACK);
        }

		for (int current_pos_y = start_point+1; current_pos_y < end_point; ++current_pos_y) {
                Screen::gotoXY(x, current_pos_y);

                if (overwrite == false) {
                    putchar(179);
                } else {
                    if (current_pos_y % CELL_HEIGHT == 3) {
                        putchar(196);
                    } else {
                        putchar(32);
                    }
                }
		}

		return;
	}

    if (first_cell.second == second_cell.second) {
        int start_point = first_cell.first;
        int end_point = second_cell.first;
        if (start_point > end_point) swapPoints(start_point, end_point);

        int y = TableObject->getYInConsole(first_cell.second);
        start_point = TableObject->getXInConsole(start_point);
        end_point = TableObject->getXInConsole(end_point);

        if (overwrite == false) {
            Screen::setConsoleColor(YELLOW, RED);
        } else {
            Screen::setConsoleColor(WHITE, BLACK);
        }

        for (int current_pos_x = start_point+1; current_pos_x < end_point; ++current_pos_x) {
            Screen::gotoXY(current_pos_x, y);

            if (overwrite == false) {
                putchar(196);
            } else {
                if (current_pos_x % CELL_LENGTH == 5) {
                    putchar(179);
                } else {
                    putchar(32);
                }
            }
        }

        return;
    }
}

bool DifficultMode::checkLMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
    if (first_cell.first == second_cell.first || first_cell.second == second_cell.second) return false;

    std::pair<int, int> corner_cell = std::pair<int, int>(first_cell.first, second_cell.second);
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            return true;
        }
    }

    corner_cell = std::pair<int, int>(second_cell.first, first_cell.second);
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            return true;
        }
    }

return false;
}

void DifficultMode::displayLLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    std::pair<int, int> corner_cell = std::pair<int, int>(first_cell.first, second_cell.second);
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            displayILine(first_cell, corner_cell, overwrite);

            Screen::gotoXY(TableObject->getXInConsole(corner_cell.first), TableObject->getYInConsole(corner_cell.second));
            if (overwrite == false) {
                std::cout<<"@";
            } else {
                std::cout<<" ";
            }

            displayILine(second_cell, corner_cell, overwrite);
            return;
        }
    }

    corner_cell = std::pair<int, int>(second_cell.first, first_cell.second);
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            displayILine(first_cell, corner_cell, overwrite);

            Screen::gotoXY(TableObject->getXInConsole(corner_cell.first), TableObject->getYInConsole(corner_cell.second));
            if (overwrite == false) {
                std::cout<<"@";
            } else {
                std::cout<<" ";
            }

            displayILine(second_cell, corner_cell, overwrite);
            return;
        }
    }
}

bool DifficultMode::checkZMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
	if (first_cell.second > second_cell.second) swapCells(first_cell, second_cell);

	for (int current_pos_y = first_cell.second + 1; current_pos_y < second_cell.second; ++current_pos_y) {
		std::pair<int, int> first_break(first_cell.first, current_pos_y);
		std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) == DELETED && getCellState(second_break.first, second_break.second) == DELETED) {
            bool first_line = checkIMatching(first_cell, first_break);
            bool second_line = checkIMatching(first_break, second_break);
            bool third_line = checkIMatching(second_break, second_cell);

            if (first_line && second_line && third_line) return true;
        }
	}

	if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

	for (int current_pos_x = first_cell.first + 1; current_pos_x < second_cell.first; ++current_pos_x) {
		std::pair<int, int> first_break(current_pos_x, first_cell.second);
		std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) == DELETED && getCellState(second_break.first, second_break.second) == DELETED) {
            bool first_line = checkIMatching(first_cell, first_break);
            bool second_line = checkIMatching(first_break, second_break);
            bool third_line = checkIMatching(second_break, second_cell);

            if (first_line && second_line && third_line) return true;
        }
	}

return false;
}

void DifficultMode::displayZLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
	if (first_cell.second > second_cell.second) swapCells(first_cell, second_cell);

	for (int current_pos_y = first_cell.second + 1; current_pos_y < second_cell.second; ++current_pos_y) {
		std::pair<int, int> first_break(first_cell.first, current_pos_y);
		std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) == DELETED && getCellState(second_break.first, second_break.second) == DELETED) {
            bool first_line = checkIMatching(first_cell, first_break);
            bool second_line = checkIMatching(first_break, second_break);
            bool third_line = checkIMatching(second_break, second_cell);

            if (first_line && second_line && third_line) {
                displayILine(first_cell, first_break, overwrite);
                Screen::gotoXY(TableObject->getXInConsole(first_break.first), TableObject->getYInConsole(first_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(first_break, second_break, overwrite);
                Screen::gotoXY(TableObject->getXInConsole(second_break.first), TableObject->getYInConsole(second_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(second_break, second_cell, overwrite);

                return;
            }
        }
	}

	if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

	for (int current_pos_x = first_cell.first + 1; current_pos_x < second_cell.first; ++current_pos_x) {
		std::pair<int, int> first_break(current_pos_x, first_cell.second);
		std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) == DELETED && getCellState(second_break.first, second_break.second) == DELETED) {
            bool first_line = checkIMatching(first_cell, first_break);
            bool second_line = checkIMatching(first_break, second_break);
            bool third_line = checkIMatching(second_break, second_cell);

            if (first_line && second_line && third_line) {
                displayILine(first_cell, first_break, overwrite);
                Screen::gotoXY(TableObject->getXInConsole(first_break.first), TableObject->getYInConsole(first_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(first_break, second_break, overwrite);
                Screen::gotoXY(TableObject->getXInConsole(second_break.first), TableObject->getYInConsole(second_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(second_break, second_cell, overwrite);

                return;
            }
        }
	}
}

bool DifficultMode::checkVerticalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.first != second_cell.first) return false;
    if (first_cell.first == 0 || first_cell.first == table_size-1) return true;


    for (int current_pos_x = first_cell.first-1; current_pos_x >= 0; --current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_x == 0 || checkIMatching(first_break, second_break) == true) return true;
    }

    for (int current_pos_x = first_cell.first+1; current_pos_x < table_size; ++current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_x == table_size-1 || checkIMatching(first_break, second_break) == true) return true;
    }

return false;
}

void DifficultMode::displayBreakPointULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell,
                                  std::pair<int, int> first_break, std::pair<int, int> second_break, bool overwrite) {
        displayILine(first_cell, first_break, overwrite);

        Screen::gotoXY(TableObject->getXInConsole(first_break.first), TableObject->getYInConsole(first_break.second));
        if (overwrite == false) {
            std::cout<<"@";
        } else {
            std::cout<<" ";
        }

        displayILine(first_break, second_break, overwrite);

        Screen::gotoXY(TableObject->getXInConsole(second_break.first), TableObject->getYInConsole(second_break.second));
        if (overwrite == false) {
            std::cout<<"@";
        } else {
            std::cout<<" ";
        }

        displayILine(second_cell, second_break, overwrite);
}

void DifficultMode::displayVerticalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.first == 0) {
        std::pair<int, int> first_break = std::pair<int, int>(-1, first_cell.second);
        std::pair<int, int> second_break = std::pair<int, int>(-1, second_cell.second);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    if (first_cell.first == table_size-1) {
        std::pair<int, int> first_break = std::pair<int, int>(table_size, first_cell.second);
        std::pair<int, int> second_break = std::pair<int, int>(table_size, second_cell.second);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    for (int current_pos_x = first_cell.first-1; current_pos_x >= 0; --current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_x == 0) {
            first_break = std::pair<int, int>(-1, first_cell.second);
            second_break = std::pair<int, int>(-1, second_cell.second);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }

    for (int current_pos_x = first_cell.first+1; current_pos_x < table_size; ++current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_x == table_size-1) {
            first_break = std::pair<int, int>(table_size, first_cell.second);
            second_break = std::pair<int, int>(table_size, second_cell.second);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }
}

bool DifficultMode::checkHorizontalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.second != second_cell.second) return false;
    if (first_cell.second == 0 || first_cell.second == table_size-1) return true;

    for (int current_pos_y = first_cell.second-1; current_pos_y >= 0; --current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_y == 0 || checkIMatching(first_break, second_break) == true) return true;
    }

    for (int current_pos_y = first_cell.second+1; current_pos_y < table_size; ++current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_y == table_size-1 || checkIMatching(first_break, second_break) == true) return true;
    }

return false;
}

void DifficultMode::displayHorizontalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.second == 0) {
        std::pair<int, int> first_break = std::pair<int, int>(first_cell.first, -1);
        std::pair<int, int> second_break = std::pair<int, int>(second_cell.first, -1);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    if (first_cell.second == table_size-1) {
        std::pair<int, int> first_break = std::pair<int, int>(first_cell.first, table_size);
        std::pair<int, int> second_break = std::pair<int, int>(second_cell.first, table_size);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    for (int current_pos_y = first_cell.second-1; current_pos_y >= 0; --current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_y == 0) {
            first_break = std::pair<int, int>(first_cell.first, -1);
            second_break = std::pair<int, int>(second_cell.first, -1);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }

    for (int current_pos_y = first_cell.second+1; current_pos_y < table_size; ++current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_y == table_size-1) {
            first_break = std::pair<int, int>(first_cell.first, table_size);
            second_break = std::pair<int, int>(second_cell.first, table_size);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }
}

bool DifficultMode::checkUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool isDisplay) {
    if (checkVerticalUMatching(first_cell, second_cell) == true) {
        if (isDisplay == true) {
            displayVerticalULine(first_cell, second_cell, false);
            Sleep(500);
            displayVerticalULine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkHorizontalUMatching(first_cell, second_cell) == true) {
        if (isDisplay == true) {
            displayHorizontalULine(first_cell, second_cell, false);
            Sleep(500);
            displayHorizontalULine(first_cell, second_cell, true);
        }
        return true;
    }

    if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

    std::pair<int, int> break_point(first_cell.first, second_cell.second);
    if (getCellState(break_point.first, break_point.second) == DELETED) {
        if (checkIMatching(first_cell, break_point) == true && checkHorizontalUMatching(second_cell, break_point) == true) {
            if (isDisplay == true) {
                displayILine(first_cell, break_point, false);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(179);

                displayHorizontalULine(second_cell, break_point, false);

                Sleep(500);

                displayILine(first_cell, break_point, true);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(32);

                displayHorizontalULine(second_cell, break_point, true);
            }
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkVerticalUMatching(first_cell, break_point) == true) {
            if (isDisplay == true) {
                displayILine(second_cell, break_point, false);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(196);

                displayVerticalULine(first_cell, break_point, false);

                Sleep(500);

                displayILine(second_cell, break_point, true);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(32);

                displayVerticalULine(first_cell, break_point, true);
            }
            return true;
        }
    }

    break_point = std::pair<int, int>(second_cell.first, first_cell.second);
    if (getCellState(break_point.first, break_point.second) == DELETED) {
        if (checkIMatching(first_cell, break_point) == true && checkVerticalUMatching(second_cell, break_point) == true) {
            if (isDisplay == true) {
                displayILine(first_cell, break_point, false);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(196);

                displayVerticalULine(second_cell, break_point, false);

                Sleep(500);

                displayILine(first_cell, break_point, true);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(32);

                displayVerticalULine(second_cell, break_point, true);
            }
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkHorizontalUMatching(first_cell, break_point) == true) {
            if (isDisplay == true) {
                displayILine(second_cell, break_point, false);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(179);

                displayHorizontalULine(first_cell, break_point, false);
                Sleep(500);

                displayILine(second_cell, break_point, true);

                Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
                putchar(32);

                displayHorizontalULine(first_cell, break_point, true);
            }
            return true;
        }
    }

return false;
}

bool DifficultMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool isDisplay) {
    if (isCharacterEqual(first_cell, second_cell) == false) return false;
    if (checkIMatching(first_cell, second_cell) == true) {
        if (isDisplay == true) {
            displayILine(first_cell, second_cell, false);
            Sleep(500);
            displayILine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkLMatching(first_cell, second_cell) == true) {
        if (isDisplay == true) {
            displayLLine(first_cell, second_cell, false);
            Sleep(500);
            displayLLine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkZMatching(first_cell, second_cell) == true) {
        if (isDisplay == true) {
            displayZLine(first_cell, second_cell, false);
            Sleep(500);
            displayZLine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkUMatching(first_cell, second_cell, isDisplay) == true) {
        if (isDisplay == true) {
            GameObject->cleanMatchingEffect();
        }
        return true;
    }

    if (isDisplay == true) {
        GameObject->cleanMatchingEffect();
    }

return false;
}

bool DifficultMode::findValidPairs() {
    for (int y = 0; y < table_size; ++y) {
        for (Cell* prev_node = TableObject->table_data[y].head; prev_node != nullptr; prev_node = prev_node->next) {
            int x = prev_node->getCellPosX();

            for (int Y = 0; Y < table_size; ++Y) {
                for (Cell* cur_node = TableObject->table_data[y].head; cur_node != nullptr; cur_node = cur_node->next) {
                    int X = cur_node->getCellPosX();

                    if (x == X && y == Y) continue;

                    if (x < 0 || y < 0) continue;
                    if (getCellState(x, y) == DELETED) continue;

                    if (X < 0 || Y < 0) continue;
                    if (getCellState(X ,Y) == DELETED) continue;

                    if (checkMatching(std::make_pair(x, y), std::make_pair(X, Y), false) == true) {
                        displayCellValueAt(x, y, PURPLE, BLACK);
                        displayCellValueAt(X, Y, PURPLE, BLACK);
                        Sleep(500);
                        displayCellValueAt(x, y, WHITE, BLACK);
                        displayCellValueAt(X, Y, WHITE, BLACK);
                        selectCell(cell_pos_x, cell_pos_y, GREEN);
                        return true;
                    }
                }
            }
        }
    }

return false;
}

