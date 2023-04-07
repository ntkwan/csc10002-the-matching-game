#include "difficult_mode.h"

DifficultMode::DifficultMode(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top,
                           Player _player, int _number_user, Player *_user_list, Player *_current_play, TableLL *_TableObject, int _mode) {
    padding_left = _padding_left;
    padding_top = _padding_top;
    mode = _mode;
    if (_TableObject == nullptr) TableObject = new TableLL(_table_size_n, _table_size_m, padding_left, padding_top);
    else {
            is_loaded = true;
            TableObject = _TableObject;
    }
    GameObject = new GameScene(_table_size_n, _table_size_m, padding_left, padding_top);
    PlayerObject = new Player(_player);
    number_user = _number_user;
    user_list = _user_list;
    mistake = (mode == CHALLENGE_MODE ? 3 : 10);

    if (_current_play == nullptr) {
        current_play.username = PlayerObject->username;
        current_play.password = PlayerObject->password;
        current_play.gamemode = (PlayerObject->gamemode == "null" ? "difficult" : PlayerObject->gamemode);
        current_play.point = (mode == CHALLENGE_MODE ? PlayerObject->point : 0);
        current_play.lvl = (mode == CHALLENGE_MODE ? PlayerObject->lvl : 0);
    } else {
        current_play.username = _current_play->username;
        current_play.password = _current_play->password;
        current_play.gamemode = "difficult";
        current_play.point = _current_play->point;
        current_play.lvl = _current_play->lvl;
    }

    table_size_n = _table_size_n;
    table_size_m = _table_size_m;
    remained_pairs = table_size_n * table_size_m;
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

    delete PlayerObject;
    PlayerObject = nullptr;
}

int DifficultMode::getCellState(int _cell_pos_x, int _cell_pos_y) const {
    Cell* node = TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x);
    if (node == nullptr) return -1;
    return node->getCellState();
}

char DifficultMode::getCellValue(int _cell_pos_x, int _cell_pos_y) const {
    Cell* node = TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x);
    if (node == nullptr) return ' ';
    return node->getCellValue();
}

void DifficultMode::setCellState(int _cell_pos_x, int _cell_pos_y, int _state)  {
    Cell* node = TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x);
    if (node == nullptr) return;
    return node->setCellState(_state);
}

void DifficultMode::setCellValue(int _cell_pos_x, int _cell_pos_y, char _value) {
    Cell* node = TableObject->table_data[_cell_pos_y].getPos(_cell_pos_x);
    if (node == nullptr) return;
    return node->setCellValue(_value);
}

char DifficultMode::getCharAt(int _cell_pos_x, int _cell_pos_y) const {
	if (getCellState(_cell_pos_x, _cell_pos_y) == DELETED) return ' ';
return getCellValue(_cell_pos_x, _cell_pos_y);
}

template<class T>
void DifficultMode::swapPoints(T &first_cell, T &second_cell) {
    T tmp = first_cell;
    first_cell = second_cell;
    second_cell = tmp;
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
    auto displayCellBackgroundAt = [](int _cell_coord_x, int _cell_coord_y, int _background, int _text, const std::string *table_img, const int pd_top, const int pd_left) {
        Screen::setConsoleColor(_background, _text);
        Screen::gotoXY(_cell_coord_x, _cell_coord_y);

        for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
            for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
                Screen::gotoXY(current_coord_x, current_coord_y);
                putchar(table_img[current_coord_y - pd_top][current_coord_x - pd_left]);
            }
        }
    };

    for (int _cell_pos_x = 0; _cell_pos_x < table_size_n; ++_cell_pos_x) {
        displayCellBackgroundAt(TableObject->getXInConsole(cell_pos_x), TableObject->getYInConsole(_cell_pos_y), WHITE, RED, GameObject->table_image, padding_top, padding_left);
    }
}

void DifficultMode::displayTableDataAtRow(int _cell_pos_y) {
    for (int _cell_pos_x = 0; _cell_pos_x < table_size_n; ++_cell_pos_x) {
        displayCellValueAt(_cell_pos_x, _cell_pos_y, WHITE, BLACK);
    }
}

void DifficultMode::deleteCell() {
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

        if (locked_list[0].second == locked_list[1].second) {

            std::pair<int, int> first_cell = locked_list[0];
            std::pair<int, int> second_cell = locked_list[1];

            if (first_cell.first > second_cell.first) swapPoints(first_cell, second_cell);

            cleanTableDataAtRow(first_cell.second);

            for (int i = 2; i >= 1; --i) {
                if (i == 2) TableObject->table_data[first_cell.second].removePos(first_cell.first);
                else TableObject->table_data[first_cell.second].removePos(second_cell.first-1);

                int x = table_size_n - 1;
                int y = first_cell.second;
                Cell* dummy_node = new Cell(' ', DELETED, TableObject->getXInConsole(x), TableObject->getYInConsole(y), x, y);

                TableObject->table_data[first_cell.second].addTail(dummy_node);
            }
        } else {

            for (auto cell : locked_list) {
                cleanTableDataAtRow(cell.second);
                TableObject->table_data[cell.second].removePos(cell.first);

                int x = table_size_n - 1;
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

void DifficultMode::moveUp() {
    if (cell_pos_y > 0) {
        unselectCell();
        --cell_pos_y;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::moveDown() {
    if (cell_pos_y < table_size_m - 1) {
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
    if (cell_pos_x < table_size_n - 1) {
        unselectCell();
        ++cell_pos_x;
        selectCell(cell_pos_x, cell_pos_y, GREEN);
    }
}

void DifficultMode::initTable() {
    if (is_loaded == false) TableObject->generateTableData();
    GameObject->displayTableBorder();
    GameObject->loadTableBackground("assets/charmander.txt");
    GameObject->displayUserInterface(85, 0, DIFFICULT_MODE);
    GameObject->loadUserData(mode, number_user, user_list, PlayerObject);
    displayTableData();
}

void DifficultMode::displayTableData() {
    for (int i = 0; i < table_size_m; ++i) {
        for (Cell* cur_node = TableObject->table_data[i].head; cur_node != nullptr; cur_node = cur_node->next) {
            if (cur_node->cell_pos_x < 0 || cur_node->cell_pos_x >= TableObject->table_data[i].list_size) continue;
            displayCellValueAt(cur_node->cell_pos_x, cur_node->cell_pos_y, WHITE, BLACK);
        }
    }

    locked_list.clear();
    selectCell(cell_pos_x, cell_pos_y, GREEN);
    Screen::setConsoleColor(WHITE, BLACK);
}

std::pair<Player, bool> DifficultMode::startGame() {
    Screen::clearConsole();
    initTable();
    while (findValidPairs(false) == false) TableObject->shuffleTableData();

    selectCell(cell_pos_x, cell_pos_y, GREEN);
    bool game_valid = true;
    while (end_loop == false && mistake > 0 && current_play.point >= 0 && remained_pairs > 0) {

        PlayerObject->point = std::max(PlayerObject->point, current_play.point);
        GameObject->displayUserAttributes(107, 3, PlayerObject, current_play, mistake);

        if (findValidPairs(false) == false) {
            GameObject->displayNotification(97, 16, "RUN OUT OF MOVES, GAME ENDS", 3000);
            game_valid = false;
            break;
        }

        switch(Screen::getConsoleInput()) {
            case 1:
                    end_loop = true;
                    break;
            case 2:
                    Screen::playSound("audio/ingame_cursor.wav");
                    moveUp();
                    break;
            case 3:
                    Screen::playSound("audio/ingame_cursor.wav");
                    moveLeft();
                    break;
            case 4:
                    Screen::playSound("audio/ingame_cursor.wav");
                    moveRight();
                    break;
            case 5:
                    Screen::playSound("audio/ingame_cursor.wav");
                    moveDown();
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
                            GameObject->displayNotification(95, 16, "NO HINTS FOUND, PLEASE SHUFFLE!!", 1000);
                        }
                    } else {
                            GameObject->displayNotification(90, 16, "YOU DON'T HAVE ENOUGHT POINTS TO USE HINTS", 1000);
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
                        GameObject->displayNotification(104, 16, "TABLE SHUFFLED!!", 1000);
                    } else {
                        GameObject->displayNotification(90, 16, "YOU DON'T HAVE ENOUGHT POINTS TO SHUFFLE", 1000);
                    }
                    break;
            case 9:
                    if (mode != CHALLENGE_MODE) {
                        Screen::playSound("audio/hint.wav");
                        GameObject->displayNotification(104, 16, "GAME SAVED!!", 1000);
                        SMenu.saveGame(current_play, TableObject);
                    } else {
                        GameObject->displayNotification(89, 16, "YOU CAN'T SAVE THE GAME IN CHALLENGE MODE", 1000);
                    }
                    break;
        }
    }
    PlayerObject->point = std::max(PlayerObject->point, current_play.point);
    GameObject->displayUserAttributes(107, 3, PlayerObject, current_play, mistake);

    if (end_loop == true) {
        if (mode == CHALLENGE_MODE) return std::make_pair(current_play, true);
    }

    ++current_play.lvl;

    if (game_valid == true) {
        setCellState(cell_pos_x, cell_pos_y, EMPTY_BOARD);
        selectCell(cell_pos_x, cell_pos_y, WHITE);
        GameObject->displayTableBackground();
    }


    if (mode != CHALLENGE_MODE) GameObject->saveUserData(number_user, current_play);
    Screen::setConsoleColor(WHITE, BLACK);
    Sleep(1500);

return {current_play, mistake <= 0};
}


bool DifficultMode::isCharacterEqual(std::pair<int,int> first_cell, std::pair<int, int> second_cell) {
    return (getCellValue(first_cell.first, first_cell.second) == getCellValue(second_cell.first, second_cell.second));
}

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

void DifficultMode::displayZLine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool DifficultMode::checkVerticalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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

    for (int current_pos_x = first_cell.first + 1; current_pos_x < table_size_n; ++current_pos_x) {
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

bool DifficultMode::checkHorizontalUMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
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

void DifficultMode::displayHorizontalULine(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
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

bool DifficultMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool is_display) {
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

bool DifficultMode::findValidPairs(bool isDisplay) {
    for (int y = 0; y < table_size_m; ++y) {
        for (Cell* prev_node = TableObject->table_data[y].head; prev_node != nullptr; prev_node = prev_node->next) {
            int x = prev_node->cell_pos_x;

            for (int Y = 0; Y < table_size_m; ++Y) {
                for (Cell* cur_node = TableObject->table_data[Y].head; cur_node != nullptr; cur_node = cur_node->next) {
                    int X = cur_node->cell_pos_x;

                    if (x == X && y == Y) continue;

                    if (x < 0) continue;
                    int cell_state = getCellState(x ,y);
                    if (cell_state == DELETED || cell_state == -1) continue;

                    if (X < 0) continue;
                    cell_state = getCellState(X ,Y);
                    if (cell_state == DELETED || cell_state == -1) continue;

                    if (checkMatching(std::make_pair(x, y), std::make_pair(X, Y), false) == true) {
                        if (isDisplay == true) {
                            displayCellValueAt(x, y, PURPLE, BLACK);
                            displayCellValueAt(X, Y, PURPLE, BLACK);
                            GameObject->displayNotification(104, 16, "HINT DISPLAYED!!", 1000);
                            displayCellValueAt(x, y, WHITE, BLACK);
                            displayCellValueAt(X, Y, WHITE, BLACK);

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

