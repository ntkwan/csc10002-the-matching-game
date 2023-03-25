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

void Game::selectCell(int _color) {
    cell_coord_x = table->getXInConsole(cell_pos_x);
    cell_coord_y = table->getYInConsole(cell_pos_y);
    Screen::gotoXY(cell_coord_x, cell_coord_y);
    Screen::setConsoleColor(_color, WHITE);

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

void Game::unSelectCell() {
    cell_coord_x = table->getXInConsole(cell_pos_x);
    cell_coord_y = table->getYInConsole(cell_pos_y);
    Screen::gotoXY(cell_coord_x, cell_coord_y);
    Screen::setConsoleColor(WHITE, BLACK);

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

void Game::moveUp() {
    if (cell_pos_y > 0) {
        unSelectCell();
        --cell_pos_y;
        selectCell(GREEN);
    }
}

void Game::moveDown() {
    if (cell_pos_y < table_size-1) {
        unSelectCell();
        ++cell_pos_y;
        selectCell(GREEN);
    }
}

void Game::moveLeft() {
    if (cell_pos_x > 0) {
        unSelectCell();
        --cell_pos_x;
        selectCell(GREEN);
    }
}

void Game::moveRight() {
    if (cell_pos_x < table_size-1) {
        unSelectCell();
        ++cell_pos_x;
        selectCell(GREEN);
    }
}

void Game::initTable() {
    table->generateTableData();
    table->displayTableData();
}

void Game::startGame() {
    Screen::clearConsole();
    initTable();

    selectCell(GREEN);

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
        }
    }

    Screen::setConsoleColor(WHITE, BLACK);
}
