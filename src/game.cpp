#include "game.h"

Game::Game() {
    table_size = table->table_size;
    remained_pairs = table_size * table_size;
    cell_pos_x = 0;
    cell_pos_y = 0;
    cell_coord_x = table->getXInConsole(cell_pos_x);
    cell_coord_y = table->getYInConsole(cell_pos_y);
}

Game::~Game() {
    delete table;
    table = nullptr;
}

void Game::selectCell(int _cell_pos_x, int _cell_pos_y, int _background) {
    int _cell_coord_x = table->getXInConsole(_cell_pos_x);
    int _cell_coord_y = table->getYInConsole(_cell_pos_y);
    Screen::gotoXY(_cell_coord_x, _cell_coord_y);
    Screen::setConsoleColor(_background, BLACK);

    for (int current_coord_y = _cell_coord_y - 1; current_coord_y <= _cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = _cell_coord_x - 3; current_coord_x <= _cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (table->table_data[_cell_pos_x][_cell_pos_y].getCellState() == DELETED) {
                table->table_data[_cell_pos_x][_cell_pos_y].setCellValue(' ');
                putchar(table->table_data[_cell_pos_x][_cell_pos_y].getCellValue());
                continue;
            }

            if (current_coord_x == _cell_coord_x && current_coord_y == _cell_coord_y) {
                putchar(table->table_data[_cell_pos_x][_cell_pos_y].getCellValue());
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
        Screen::setConsoleColor(WHITE, BLACK);
    }

    for (int current_coord_y = cell_coord_y - 1; current_coord_y <= cell_coord_y + 1; ++current_coord_y) {
        for (int current_coord_x = cell_coord_x - 3; current_coord_x <= cell_coord_x + 3; ++current_coord_x) {
            Screen::gotoXY(current_coord_x, current_coord_y);

            if (current_coord_x == cell_coord_x && current_coord_y == cell_coord_y) {
                putchar(table->table_data[cell_pos_x][cell_pos_y].getCellValue());
            }
            else {
                putchar(' ');
            }
        }
    }

    Screen::gotoXY(cell_coord_x, cell_coord_y);
}

void Game::deleteCell() {
    if (checkMatching(lockedList[0], lockedList[1]) == false) {
        for (auto cell : lockedList) {
            Screen::gotoXY(table->getXInConsole(cell.first), table->getYInConsole(cell.second));
            table->table_data[cell.first][cell.second].setCellState(FREE);
            selectCell(cell.first, cell.second, WHITE);
        }
        lockedList.clear();
    } else {
        remained_pairs -= 2;

        for (auto cell : lockedList) {
            Screen::gotoXY(table->getXInConsole(cell.first), table->getYInConsole(cell.second));
            table->table_data[cell.first][cell.second].setCellState(DELETED);
            selectCell(cell.first, cell.second, WHITE);
        }
        lockedList.clear();
    }
}
void Game::lockCell() {
    int cell_state = table->table_data[cell_pos_x][cell_pos_y].getCellState();

    if (cell_state == LOCKED || cell_state == DELETED) return;

    selectCell(cell_pos_x, cell_pos_y, YELLOW);
    table->table_data[cell_pos_x][cell_pos_y].setCellState(LOCKED);
    lockedList.push_back(std::pair<int, int>(cell_pos_x, cell_pos_y));

    if (lockedList.size() == 2) {
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
}

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

    bool flag = false;
    while (flag == false) {
        switch(Screen::getConsoleInput()) {
            case 1:
                    flag = true;
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

		for (int i = start_point+1; i < end_point; ++i) {
			if (table->table_data[first_cell.first][i].getCellState() != DELETED) return false;
		}
		return true;
	}

	if (first_cell.second == second_cell.second) {
		int start_point = first_cell.second;
		int end_point = second_cell.second;
		if (start_point > end_point) swapPoints(start_point, end_point);

		for (int i = start_point+1; i < end_point; ++i) {
			if (table->table_data[i][first_cell.second].getCellState() != DELETED) return false;
		}
		return true;
	}

return false;
}

bool Game::checkLMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
    std::pair<int, int> corner_cell = std::pair<int, int>(first_cell.first, second_cell.second);

    if (table->table_data[corner_cell.first][corner_cell.second].getCellState() == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            return true;
        }
    }

    corner_cell = std::pair<int, int>(second_cell.first, first_cell.second);
    if (table->table_data[corner_cell.first][corner_cell.second].getCellState() == DELETED) {
        if (checkIMatching(first_cell, corner_cell) == true && checkIMatching(second_cell, corner_cell) == true) {
            return true;
        }
    }

return false;
}

bool Game::checkZMatching(std::pair<int, int> first_cell, std::pair<int,int> second_cell) {
	if (first_cell.second > second_cell.second) swapCells(first_cell, second_cell);

	for (int current_pos_y = first_cell.second + 1; current_pos_y < second_cell.second; ++current_pos_y) {
		std::pair<int, int> first_break(first_cell.first, current_pos_y);
		std::pair<int, int> second_break(second_cell.first, current_pos_y);

		bool first_line = checkIMatching(first_cell, first_break);
		bool second_line = checkIMatching(first_break, second_break);
		bool third_line = checkIMatching(second_break, second_cell);

		if (first_line && second_line && third_line) return true;
	}

	if (first_cell.first > second_cell.first) swapCells(first_cell, second_cell);

	for (int current_pos_x = first_cell.first + 1; current_pos_x < second_cell.first; ++current_pos_x) {
		std::pair<int, int> first_break(current_pos_x, first_cell.second);
		std::pair<int, int> second_break(current_pos_x, second_cell.second);

		bool first_line = checkIMatching(first_cell, first_break);
		bool second_line = checkIMatching(first_break, second_break);
		bool third_line = checkIMatching(second_break, second_cell);

		if (first_line && second_line && third_line) return true;
	}

return false;
}

bool Game::checkMatching(std::pair<int, int> firstCell, std::pair<int, int> secondCell) {
    if (isCharacterEqual(firstCell, secondCell) == false) return false;
    if (checkIMatching(firstCell, secondCell) == true) return true;
    if (checkLMatching(firstCell, secondCell) == true) return true;
    if (checkZMatching(firstCell, secondCell) == true) return true;
return false;
}
