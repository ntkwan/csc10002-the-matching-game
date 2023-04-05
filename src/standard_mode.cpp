#include "standard_mode.h"

StandardMode::StandardMode(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top, Player _player, int _number_user, Player *_user_list) {
    padding_left = _padding_left;
    padding_top = _padding_top;

    TableObject = new Table(_table_size_n, _table_size_m, padding_left, padding_top);
    GameObject = new GameScene(_table_size_n, _table_size_m, padding_left, padding_top);
    PlayerObject = new Player(_player);
    number_user = _number_user;
    user_list = _user_list;

    current_play.username = PlayerObject->username;
    current_play.password = PlayerObject->password;
    current_play.gamemode = "standard";
    current_play.point = 0;
    current_play.lvl = 0;

    table_size_n = _table_size_n;
    table_size_m = _table_size_m;
    remained_pairs = table_size_n * table_size_m;
    cell_pos_x = 0;
    cell_pos_y = 0;
    cell_coord_x = TableObject->getXInConsole(cell_pos_x);
    cell_coord_y = TableObject->getYInConsole(cell_pos_y);
    end_loop = false;
}

StandardMode::~StandardMode() {
    delete TableObject;
    TableObject = nullptr;

    delete GameObject;
    GameObject = nullptr;
}

int StandardMode::getCellState(int _cell_pos_x, int _cell_pos_y) const { return TableObject->table_data[_cell_pos_x][_cell_pos_y].getCellState(); }

char StandardMode::getCellValue(int _cell_pos_x, int _cell_pos_y) const { return TableObject->table_data[_cell_pos_x][_cell_pos_y].getCellValue(); }

void StandardMode::setCellState(int _cell_pos_x, int _cell_pos_y, int _state)  { TableObject->table_data[_cell_pos_x][_cell_pos_y].setCellState(_state); }

void StandardMode::setCellValue(int _cell_pos_x, int _cell_pos_y, char _value) { TableObject->table_data[_cell_pos_x][_cell_pos_y].setCellValue(_value); }

template<class T>
void StandardMode::swapPoints(T &first_cell, T &second_cell) {
    T tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
}

char StandardMode::getCharAt(int _cell_pos_x, int _cell_pos_y) const {
	if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED) return ' ';
return getCellValue(_cell_pos_x, _cell_pos_y);
}

void StandardMode::selectCell(int _cell_pos_x, int _cell_pos_y, int _background) {
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

void StandardMode::unselectCell() {
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

void StandardMode::deleteCell() {
    if (checkMatching(locked_list[0], locked_list[1], true) == false) {
        Screen::playSound("audio/mismatched.wav");
        mistake--;
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
        Screen::playSound("audio/matched.wav");
        remained_pairs -= 2;

        for (auto cell : locked_list) {
            setCellState(cell.first, cell.second, DELETED);
            selectCell(cell.first, cell.second, WHITE);
        }
        locked_list.clear();
    }
}

void StandardMode::lockCell() {
    int cell_state = getCellState(cell_pos_x, cell_pos_y);

    if (cell_state == DELETED) return;

    if (locked_list.empty() == false && cell_pos_x == locked_list[0].first && cell_pos_y == locked_list[0].second) {
        locked_list.pop_back();
        setCellState(cell_pos_x, cell_pos_y, FREE);
        selectCell(cell_pos_x, cell_pos_y, GREEN);
        return;
    }

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

void StandardMode::moveUp() {
    if (cell_pos_y > 0) {
        unselectCell();
        --cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void StandardMode::moveDown() {
    if (cell_pos_y < table_size_m - 1) {
        unselectCell();
        ++cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void StandardMode::moveLeft() {
    if (cell_pos_x > 0) {
        unselectCell();
        --cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void StandardMode::moveRight() {
    if (cell_pos_x < table_size_n - 1) {
        unselectCell();
        ++cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void StandardMode::initTable() {
    TableObject->generateTableData();
    GameObject->displayTableBorder();
    GameObject->loadTableBackground("assets/bunny.txt");
    GameObject->displayUserInterface(70, 0, STANDARD_MODE);
    GameObject->loadUserData(STANDARD_MODE, number_user, user_list, PlayerObject);
    displayTableData();
}

void StandardMode::displayTableData() {
    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            displayCellValueAt(TableObject->table_data[i][j].cell_pos_x, TableObject->table_data[i][j].cell_pos_y, WHITE, BLACK);
        }
    }

    locked_list.clear();
    selectCell(cell_pos_x, cell_pos_y, GREEN);
    Screen::setConsoleColor(WHITE, BLACK);
}

void StandardMode::startGame() {
    Screen::clearConsole();
    initTable();
    selectCell(cell_pos_x, cell_pos_y, GREEN);
    bool game_valid = true;
    while (end_loop == false && mistake > 0 && current_play.point >= 0 && remained_pairs > 0) {

        PlayerObject->point = std::max(PlayerObject->point, current_play.point);
        GameObject->displayUserAttributes(92, 3, PlayerObject, current_play, mistake);

        if (findValidPairs(false) == false) {
            GameObject->displayNotification(80, 16, "RUN OUT OF MOVES, GAME ENDS", 3000);
            game_valid = false;
            break;
        }

        switch(Screen::getConsoleInput()) {
            case 1:
                    end_loop = true;
                    break;
            case 2:
                    moveUp();
                    Screen::playSound("audio/ingame_cursor.wav");
                    break;
            case 3:
                    moveLeft();
                    Screen::playSound("audio/ingame_cursor.wav");
                    break;
            case 4:
                    moveRight();
                    Screen::playSound("audio/ingame_cursor.wav");
                    break;
            case 5:
                    moveDown();
                    Screen::playSound("audio/ingame_cursor.wav");
                    break;
            case 6:
                    Screen::playSound("audio/click.wav");
                    lockCell();
                    break;
            case 7:
                    if (current_play.point-5 >= 0) {
                        current_play.point -= 5;
                        Screen::playSound("audio/hint.wav");
                        if (findValidPairs(true) == false) {
                            GameObject->displayNotification(80, 16, "NO HINTS FOUND, PLEASE SHUFFLE!!", 1000);
                        }
                    } else {
                            GameObject->displayNotification(75, 16, "YOU DON'T HAVE ENOUGHT POINTS TO USE HINTS", 1000);
                    }
                    break;
            case 8:
                    if (current_play.point-9 >= 0) {
                        current_play.point -= 9;
                        Screen::playSound("audio/shuffle.wav");
                        TableObject->shuffleTableData();
                        while (findValidPairs(false) == false) {
                            TableObject->shuffleTableData();
                        }
                        displayTableData();
                        GameObject->displayNotification(89, 16, "TABLE SHUFFLED!!", 1000);
                    } else {
                        GameObject->displayNotification(75, 16, "YOU DON'T HAVE ENOUGHT POINTS TO SHUFFLE", 1000);
                    }
                    break;
        }
    }
    PlayerObject->point = std::max(PlayerObject->point, current_play.point);
    GameObject->displayUserAttributes(92, 3, PlayerObject, current_play, mistake);

    ++current_play.lvl;

    if (game_valid == true) {
        setCellState(cell_pos_x, cell_pos_y, EMPTY_BOARD);
        selectCell(cell_pos_x, cell_pos_y, WHITE);
        GameObject->displayTableBackground();
    }


    GameObject->saveUserData(number_user, current_play);
    Screen::setConsoleColor(WHITE, BLACK);
}

bool StandardMode::isCharacterEqual(std::pair<int,int> first_cell, std::pair<int, int> second_cell) { return (getCellValue(first_cell.first, first_cell.second) == getCellValue(second_cell.first, second_cell.second)); }

bool StandardMode::checkIMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
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

void StandardMode::displayILine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool StandardMode::checkLMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
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

void StandardMode::displayLLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool StandardMode::checkZMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
	if (first_cell.second > second_cell.second) swapPoints(first_cell, second_cell);

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

	if (first_cell.first > second_cell.first) swapPoints(first_cell, second_cell);

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

void StandardMode::displayZLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
	if (first_cell.second > second_cell.second) swapPoints(first_cell, second_cell);

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

	if (first_cell.first > second_cell.first) swapPoints(first_cell, second_cell);

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

bool StandardMode::checkVerticalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.first != second_cell.first) return false;
    if (first_cell.first == 0 || first_cell.first == table_size_n - 1) return true;


    for (int current_pos_x = first_cell.first-1; current_pos_x >= 0; --current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_x == 0 || checkIMatching(first_break, second_break) == true) return true;
    }

    for (int current_pos_x = first_cell.first+1; current_pos_x < table_size_n; ++current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_x == table_size_n - 1 || checkIMatching(first_break, second_break) == true) return true;
    }

return false;
}

void StandardMode::displayBreakPointULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell,
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

void StandardMode::displayVerticalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.first == 0) {
        std::pair<int, int> first_break = std::pair<int, int>(-1, first_cell.second);
        std::pair<int, int> second_break = std::pair<int, int>(-1, second_cell.second);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    if (first_cell.first == table_size_n - 1) {
        std::pair<int, int> first_break = std::pair<int, int>(table_size_n, first_cell.second);
        std::pair<int, int> second_break = std::pair<int, int>(table_size_n, second_cell.second);
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

    for (int current_pos_x = first_cell.first+1; current_pos_x < table_size_n; ++current_pos_x) {
        std::pair<int, int> first_break(current_pos_x, first_cell.second);
        std::pair<int, int> second_break(current_pos_x, second_cell.second);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_x == table_size_n - 1) {
            first_break = std::pair<int, int>(table_size_n, first_cell.second);
            second_break = std::pair<int, int>(table_size_n, second_cell.second);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }
}

bool StandardMode::checkHorizontalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.second != second_cell.second) return false;
    if (first_cell.second == 0 || first_cell.second == table_size_m - 1) return true;

    for (int current_pos_y = first_cell.second-1; current_pos_y >= 0; --current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_y == 0 || checkIMatching(first_break, second_break) == true) return true;
    }

    for (int current_pos_y = first_cell.second+1; current_pos_y < table_size_m; ++current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;
        if (current_pos_y == table_size_m - 1 || checkIMatching(first_break, second_break) == true) return true;
    }

return false;
}

void StandardMode::displayHorizontalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.second == 0) {
        std::pair<int, int> first_break = std::pair<int, int>(first_cell.first, -1);
        std::pair<int, int> second_break = std::pair<int, int>(second_cell.first, -1);
        displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
        return;
    }

    if (first_cell.second == table_size_m - 1) {
        std::pair<int, int> first_break = std::pair<int, int>(first_cell.first, table_size_m);
        std::pair<int, int> second_break = std::pair<int, int>(second_cell.first, table_size_m);
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

    for (int current_pos_y = first_cell.second+1; current_pos_y < table_size_m; ++current_pos_y) {
        std::pair<int, int> first_break(first_cell.first, current_pos_y);
        std::pair<int, int> second_break(second_cell.first, current_pos_y);

        if (getCellState(first_break.first, first_break.second) != DELETED || getCellState(second_break.first, second_break.second) != DELETED) break;

        if (checkIMatching(first_break, second_break) == true) {
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        } else if (current_pos_y == table_size_m - 1) {
            first_break = std::pair<int, int>(first_cell.first, table_size_m);
            second_break = std::pair<int, int>(second_cell.first, table_size_m);
            displayBreakPointULine(first_cell, second_cell, first_break, second_break, overwrite);
            return;
        }
    }
}

bool StandardMode::checkUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool isDisplay) {
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

    if (first_cell.first > second_cell.first) swapPoints(first_cell, second_cell);

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

bool StandardMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool is_display) {
    if (isCharacterEqual(first_cell, second_cell) == false) return false;
    if (checkIMatching(first_cell, second_cell) == true) {
        if (is_display == true) {
            current_play.point += 3;
            displayILine(first_cell, second_cell, false);
            Sleep(500);
            displayILine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkLMatching(first_cell, second_cell) == true) {
        if (is_display == true) {
            current_play.point += 5;
            displayLLine(first_cell, second_cell, false);
            Sleep(500);
            displayLLine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkZMatching(first_cell, second_cell) == true) {
        if (is_display == true) {
            current_play.point += 7;
            displayZLine(first_cell, second_cell, false);
            Sleep(500);
            displayZLine(first_cell, second_cell, true);
        }
        return true;
    }

    if (checkUMatching(first_cell, second_cell, is_display) == true) {
        if (is_display == true) {
            current_play.point += 7;
            GameObject->cleanMatchingEffect();
        }
        return true;
    }

    if (is_display == true) {
        GameObject->cleanMatchingEffect();
    }

return false;
}

void StandardMode::displayCellValueAt(int _cell_pos_x, int _cell_pos_y, int _background, int _text) {
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

bool StandardMode::findValidPairs(bool isDisplay) {
    for (int x = 0; x < table_size_n; ++x) {
        for (int y = 0; y < table_size_m; ++y) {
            if (getCellState(x, y) == DELETED) continue;
            char firstCellValue = getCellValue(x, y);

            for (int r = x; r < table_size_n; ++r) {
                for (int c = 0; c < table_size_m; ++c) {
                    if (x == r && c <= y) continue;
                    if (getCellState(r, c) == DELETED) continue;
                    char secondCellValue = getCellValue(r, c);

                    if (firstCellValue != secondCellValue) continue;

                    if (checkMatching(std::make_pair(x, y), std::make_pair(r, c), false) == true) {
                        if (isDisplay == true) {
                            displayCellValueAt(x, y, PURPLE, BLACK);
                            displayCellValueAt(r, c, PURPLE, BLACK);
                            GameObject->displayNotification(89, 16, "HINT DISPLAYED!!", 1000);
                            displayCellValueAt(x, y, WHITE, BLACK);
                            displayCellValueAt(r, c, WHITE, BLACK);

                            for (auto cell : locked_list) {
                                displayCellValueAt(cell.first, cell.second, YELLOW, BLACK);
                            }
                            selectCell(cell_pos_x, cell_pos_y, GREEN);
                        }
                        return true;
                    }
                }
            }

        }
    }

return false;
}
