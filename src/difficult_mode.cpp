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

void DifficultMode::displayCellAt(int _cell_pos_x, int _cell_pos_y, int _background, int _text) {
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

void DifficultMode::cleanTableAtRow(int _cell_pos_y) {
    for (int _cell_pos_x = 0; _cell_pos_x < table_size; ++_cell_pos_x) {
        displayCellAt(_cell_pos_x, _cell_pos_y, WHITE, RED);
    }
}

void DifficultMode::deleteCell() {
    if (checkMatching(locked_list[0], locked_list[1]) == false) {
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

        for (auto cell : locked_list) {
            cleanTableAtRow(cell.second);
            setCellState(cell.first, cell.second, DELETED);
            TableObject->table_data[cell.second].removePos(cell.first);

            int x = table_size-1;
            int y = cell.first;

            Cell* cur_node = new Cell;
            cur_node->cell_value = ' ';
            cur_node->cell_state = EMPTY_BOARD;
            cur_node->cell_coord_x = TableObject->getXInConsole(x);
            cur_node->cell_coord_y = TableObject->getYInConsole(y);
            cur_node->cell_pos_x = x;
            cur_node->cell_pos_y = y;
            TableObject->table_data[cell.second].addTail(cur_node);
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
            first_break = std::pair<int, int>(-1, first_cell.second);
            second_break = std::pair<int, int>(-1, second_cell.second);
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

bool DifficultMode::checkUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (checkVerticalUMatching(first_cell, second_cell) == true) {
        displayVerticalULine(first_cell, second_cell, false);
        Sleep(500);
        displayVerticalULine(first_cell, second_cell, true);
        return true;
    }

    if (checkHorizontalUMatching(first_cell, second_cell) == true) {
        displayHorizontalULine(first_cell, second_cell, false);
        Sleep(500);
        displayHorizontalULine(first_cell, second_cell, true);
        return true;
    }

    if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

    std::pair<int, int> break_point(first_cell.first, second_cell.second);
    if (getCellState(break_point.first, break_point.second) == DELETED) {
        if (checkIMatching(first_cell, break_point) == true && checkHorizontalUMatching(second_cell, break_point) == true) {
            displayILine(first_cell, break_point, false);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(179);

            displayHorizontalULine(second_cell, break_point, false);

            Sleep(500);

            displayILine(first_cell, break_point, true);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(32);

            displayHorizontalULine(second_cell, break_point, true);
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkVerticalUMatching(first_cell, break_point) == true) {
            displayILine(second_cell, break_point, false);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(196);

            displayVerticalULine(first_cell, break_point, false);

            Sleep(500);

            displayILine(second_cell, break_point, true);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(32);

            displayVerticalULine(first_cell, break_point, true);
            return true;
        }
    }

    break_point = std::pair<int, int>(second_cell.first, first_cell.second);
    if (getCellState(break_point.first, break_point.second) == DELETED) {
        if (checkIMatching(first_cell, break_point) == true && checkVerticalUMatching(second_cell, break_point) == true) {
            displayILine(first_cell, break_point, false);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(196);

            displayVerticalULine(second_cell, break_point, false);

            Sleep(500);

            displayILine(first_cell, break_point, true);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(32);

            displayVerticalULine(second_cell, break_point, true);
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkHorizontalUMatching(first_cell, break_point) == true) {
            displayILine(second_cell, break_point, false);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(179);

            displayHorizontalULine(first_cell, break_point, false);
            Sleep(500);

            displayILine(second_cell, break_point, true);

            Screen::gotoXY(TableObject->getXInConsole(break_point.first), TableObject->getYInConsole(break_point.second));
            putchar(32);

            displayHorizontalULine(first_cell, break_point, true);
            return true;
        }
    }

return false;
}

bool DifficultMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (isCharacterEqual(first_cell, second_cell) == false) return false;
    if (checkIMatching(first_cell, second_cell) == true) {
        displayILine(first_cell, second_cell, false);
        Sleep(500);
        displayILine(first_cell, second_cell, true);
        return true;
    }

    if (checkLMatching(first_cell, second_cell) == true) {
        displayLLine(first_cell, second_cell, false);
        Sleep(500);
        displayLLine(first_cell, second_cell, true);
        return true;
    }

    if (checkZMatching(first_cell, second_cell) == true) {
        displayZLine(first_cell, second_cell, false);
        Sleep(500);
        displayZLine(first_cell, second_cell, true);
        return true;
    }

    if (checkUMatching(first_cell, second_cell) == true) {
        GameObject->cleanMatchingEffect();
        return true;
    }
    GameObject->cleanMatchingEffect();

return false;
}
