#include "game.h"

Game::Game(int _mode, int _padding_left, int _padding_top) {
    mode = _mode;
    padding_left = _padding_left;
    padding_top = _padding_top;

    table = new Table(mode, padding_left, padding_top);

    table_size = table->table_size;
    remained_pairs = table_size * table_size;
    cell_pos_x = 0;
    cell_pos_y = 0;
    cell_coord_x = table->getXInConsole(cell_pos_x);
    cell_coord_y = table->getYInConsole(cell_pos_y);
    end_loop = false;
}

Game::~Game() {
    delete table;
    table = nullptr;
}

char Game::getCharAt(int r, int c) const {
	if (table->table_data[r][c].getCellState() == DELETED) return ' ';
	return table->table_data[r][c].getCellValue();
}

void Game::selectCell(int _cell_pos_x, int _cell_pos_y, int _background) {
    int _cell_coord_x = table->getXInConsole(_cell_pos_x);
    int _cell_coord_y = table->getYInConsole(_cell_pos_y);
    Screen::gotoXY(_cell_coord_x, _cell_coord_y);

    if (table->table_data[cell_pos_x][cell_pos_y].getCellState() == DELETED) {
        Screen::setConsoleColor(GREEN, RED);
    } else {
        if (table->table_data[cell_pos_x][cell_pos_y].getCellState() == LOCKED || table->table_data[cell_pos_x][cell_pos_y].getCellState() == EMPTY_BOARD) {
            Screen::setConsoleColor(_background, RED);
        } else {
            Screen::setConsoleColor(_background, BLACK);
        }
    }

    for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED || getCellState(_cell_pos_x, _cell_pos_y) == EMPTY_BOARD) {
                putchar(table->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
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

void Game::unSelectCell() {
    cell_coord_x = table->getXInConsole(cell_pos_x);
    cell_coord_y = table->getYInConsole(cell_pos_y);
    Screen::gotoXY(cell_coord_x, cell_coord_y);

    if (table->table_data[cell_pos_x][cell_pos_y].getCellState() == LOCKED) {
        Screen::setConsoleColor(YELLOW, BLACK);
    } else {
        if (table->table_data[cell_pos_x][cell_pos_y].getCellState() == DELETED) {
            Screen::setConsoleColor(WHITE, RED);
        } else {
            Screen::setConsoleColor(WHITE, BLACK);
        }
    }

    for (int current_coord_y = cell_coord_y - 1; current_coord_y <= cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = cell_coord_x - 3; current_coord_x <= cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (getCellState(cell_pos_x, cell_pos_y) == DELETED) {
                putchar(table->table_image[current_coord_y - padding_top][current_coord_x - padding_left]);
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

void Game::deleteCell() {
    if (checkMatching(locked_list[0], locked_list[1]) == false) {
        reverse(locked_list.begin(), locked_list.end());
        for (auto cell : locked_list) {
            Screen::gotoXY(table->getXInConsole(cell.first), table->getYInConsole(cell.second));
            setCellState(cell.first, cell.second, FREE);
            selectCell(cell.first, cell.second, RED);
        }

        Sleep(500);

        for (auto cell : locked_list) {
            Screen::gotoXY(table->getXInConsole(cell.first), table->getYInConsole(cell.second));
            selectCell(cell.first, cell.second, WHITE);
        }

        locked_list.clear();
    } else {
        remained_pairs -= 2;

        for (auto cell : locked_list) {
            table->table_data[cell.first][cell.second].setCellState(DELETED);
            selectCell(cell.first, cell.second, WHITE);
        }
        locked_list.clear();
    }
}

void Game::lockCell() {
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

void Game::moveUp() {
    if (cell_pos_y > 0) {
        unSelectCell();
        --cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void Game::moveDown() {
    if (cell_pos_y < table_size-1) {
        unSelectCell();
        ++cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void Game::moveLeft() {
    if (cell_pos_x > 0) {
        unSelectCell();
        --cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void Game::moveRight() {
    if (cell_pos_x < table_size-1) {
        unSelectCell();
        ++cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void Game::initTable() {
    table->generateTableData();
    table->displayTableData();
    table->loadTableBackground("assets/bunny.txt");
}

int Game::getCellState(int _cell_pos_x, int _cell_pos_y) const { return table->table_data[_cell_pos_x][_cell_pos_y].getCellState(); }

char Game::getCellValue(int _cell_pos_x, int _cell_pos_y) const { return table->table_data[_cell_pos_x][_cell_pos_y].getCellValue(); }

void Game::setCellState(int _cell_pos_x, int _cell_pos_y, int _state)  { table->table_data[_cell_pos_x][_cell_pos_y].setCellState(_state); }

void Game::setCellValue(int _cell_pos_x, int _cell_pos_y, char _value) { table->table_data[_cell_pos_x][_cell_pos_y].setCellValue(_value); }

void Game::swapPoints(int &first_cell, int &second_cell) {
    int tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
}

void Game::swapCells(std::pair<int, int> &first_cell, std::pair<int, int> &second_cell) {
    std::pair<int, int> tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
}

void Game::startGame() {
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

    table->table_data[cell_pos_x][cell_pos_y].setCellState(EMPTY_BOARD);
    selectCell(cell_pos_x, cell_pos_y, WHITE);
    table->displayTableBackground();
    Screen::setConsoleColor(WHITE, BLACK);
}

bool Game::isCharacterEqual(std::pair<int,int> first_cell, std::pair<int, int> second_cell) {
    return (table->table_data[first_cell.first][first_cell.second].getCellValue() ==
            table->table_data[second_cell.first][second_cell.second].getCellValue());
}

bool Game::checkIMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
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

void Game::displayILine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
	if (first_cell.first == second_cell.first) {
		int start_point = first_cell.second;
		int end_point = second_cell.second;
		if (start_point > end_point) swapPoints(start_point, end_point);

        int x = table->getXInConsole(first_cell.first);
        start_point = table->getYInConsole(start_point);
        end_point = table->getYInConsole(end_point);

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

        int y = table->getYInConsole(first_cell.second);
        start_point = table->getXInConsole(start_point);
        end_point = table->getXInConsole(end_point);

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

bool Game::checkLMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
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

void Game::displayLLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    std::pair<int, int> corner_cell = std::pair<int, int>(first_cell.first, second_cell.second);
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            displayILine(first_cell, corner_cell, overwrite);

            Screen::gotoXY(table->getXInConsole(corner_cell.first), table->getYInConsole(corner_cell.second));
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

            Screen::gotoXY(table->getXInConsole(corner_cell.first), table->getYInConsole(corner_cell.second));
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

bool Game::checkZMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
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

void Game::displayZLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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
                Screen::gotoXY(table->getXInConsole(first_break.first), table->getYInConsole(first_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(first_break, second_break, overwrite);
                Screen::gotoXY(table->getXInConsole(second_break.first), table->getYInConsole(second_break.second));
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
                Screen::gotoXY(table->getXInConsole(first_break.first), table->getYInConsole(first_break.second));
                if (overwrite == false) {
                    std::cout<<"@";
                } else {
                    std::cout<<" ";
                }

                displayILine(first_break, second_break, overwrite);
                Screen::gotoXY(table->getXInConsole(second_break.first), table->getYInConsole(second_break.second));
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

bool Game::checkVerticalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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

void Game::displayBreakPointULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell,
                                  std::pair<int, int> first_break, std::pair<int, int> second_break, bool overwrite) {
        displayILine(first_cell, first_break, overwrite);

        Screen::gotoXY(table->getXInConsole(first_break.first), table->getYInConsole(first_break.second));
        if (overwrite == false) {
            std::cout<<"@";
        } else {
            std::cout<<" ";
        }

        displayILine(first_break, second_break, overwrite);

        Screen::gotoXY(table->getXInConsole(second_break.first), table->getYInConsole(second_break.second));
        if (overwrite == false) {
            std::cout<<"@";
        } else {
            std::cout<<" ";
        }

        displayILine(second_cell, second_break, overwrite);
}

void Game::displayVerticalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool Game::checkHorizontalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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

void Game::displayHorizontalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool Game::checkUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(179);

            displayHorizontalULine(second_cell, break_point, false);

            Sleep(500);

            displayILine(first_cell, break_point, true);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(32);

            displayHorizontalULine(second_cell, break_point, true);
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkVerticalUMatching(first_cell, break_point) == true) {
            displayILine(second_cell, break_point, false);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(196);

            displayVerticalULine(first_cell, break_point, false);

            Sleep(500);

            displayILine(second_cell, break_point, true);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(32);

            displayVerticalULine(first_cell, break_point, true);
            return true;
        }
    }

    break_point = std::pair<int, int>(second_cell.first, first_cell.second);
    if (getCellState(break_point.first, break_point.second) == DELETED) {
        if (checkIMatching(first_cell, break_point) == true && checkVerticalUMatching(second_cell, break_point) == true) {
            displayILine(first_cell, break_point, false);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(196);

            displayVerticalULine(second_cell, break_point, false);

            Sleep(500);

            displayILine(first_cell, break_point, true);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(32);

            displayVerticalULine(second_cell, break_point, true);
            return true;
        }

        if (checkIMatching(second_cell, break_point) == true && checkHorizontalUMatching(first_cell, break_point) == true) {
            displayILine(second_cell, break_point, false);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(179);

            displayHorizontalULine(first_cell, break_point, false);
            Sleep(500);

            displayILine(second_cell, break_point, true);

            Screen::gotoXY(table->getXInConsole(break_point.first), table->getYInConsole(break_point.second));
            putchar(32);

            displayHorizontalULine(first_cell, break_point, true);
            return true;
        }
    }

return false;
}

bool Game::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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
        table->cleanMatchingEffect();
        return true;
    }
    table->cleanMatchingEffect();

return false;
}
