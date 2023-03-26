#ifndef GAME_H
#define GAME_H

#include <vector>

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
    std::vector<std::pair<int, int>> lockedList;

    Game();
    ~Game();

    void initTable();

    void swapPoints(int &, int &);
    void swapCells(std::pair<int, int> &, std::pair<int, int> &);
    void selectCell(int, int, int);
    void unSelectCell();
    void lockCell();
    void deleteCell();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void startGame();
    bool isCharacterEqual(std::pair<int, int>, std::pair<int, int>);
    bool checkMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkIMatching(std::pair<int,int>, std::pair<int, int>);
    bool checkLMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkZMatching(std::pair<int, int>, std::pair<int, int>);
};

#endif // GAME_H
