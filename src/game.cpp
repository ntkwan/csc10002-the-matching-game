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

void Game::swapCells(int &A, int &B) {
    int tmp = A;
    A = B;
    B = tmp;
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

bool Game::isCharacterEqual(std::pair<int,int> firstCell, std::pair<int, int> secondCell) {
	return (table->table_data[firstCell.first][firstCell.second].getCellValue() ==
            table->table_data[secondCell.first][secondCell.second].getCellValue());
}

bool Game::checkIMatching(std::pair<int, int> firstCell, std::pair<int,int> secondCell) {
	if (firstCell.first == secondCell.first) {
		int start_point = firstCell.second;
		int end_point = secondCell.second;
		if (start_point > end_point) swapCells(start_point, end_point);

		for (int i = start_point+1; i < end_point; ++i) {
			if (table->table_data[firstCell.first][i].getCellState() != DELETED) return false;
		}
		return true;
	}

	if (firstCell.second == secondCell.second) {
		int start_point = firstCell.second;
		int end_point = secondCell.second;
		if (start_point > end_point) swapCells(start_point, end_point);

		for (int i = start_point+1; i < end_point; ++i) {
			if (table->table_data[i][firstCell.second].getCellState() != DELETED) return false;
		}
		return true;
	}

	return false;
}

bool Game::checkLMatching(std::pair<int, int> firstCell, std::pair<int,int> secondCell) {
    std::pair<int, int> cornerCell = std::pair<int, int>(firstCell.first, secondCell.second);

    if (table->table_data[cornerCell.first][cornerCell.second].getCellState() == DELETED) {
        if (checkIMatching(firstCell, cornerCell) == true && checkIMatching(secondCell, cornerCell) == true) {
            return true;
        }
    }

    cornerCell = std::pair<int, int>(secondCell.first, firstCell.second);
    if (table->table_data[cornerCell.first][cornerCell.second].getCellState() == DELETED) {
        if (checkIMatching(firstCell, cornerCell) == true && checkIMatching(secondCell, cornerCell) == true) {
            return true;
        }
    }

return false;
}

bool Game::checkMatching(std::pair<int, int> firstCell, std::pair<int, int> secondCell) {
    if (isCharacterEqual(firstCell, secondCell) == false) return false;
    if (checkIMatching(firstCell, secondCell) == true) return true;
    if (checkLMatching(firstCell, secondCell) == true) return true;
return false;
}
