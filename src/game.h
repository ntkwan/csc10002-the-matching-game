#ifndef GAME_H
#define GAME_H

#include "table.h"

struct Game {
    int padding_left = 20;
    int padding_top = 3;
    Table *table = new Table(4, padding_left, padding_top);

    int table_size;
    int remained_pairs;
    int cell_pos_x;
    int cell_pos_y;
    int cell_coord_x;
    int cell_coord_y;

    Game();
    ~Game();

    void initTable();
    void selectCell(int _color);
    void unSelectCell();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void startGame();
};

#endif // GAME_H
