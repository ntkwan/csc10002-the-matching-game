#ifndef DIFFICULT_MODE_H
#define DIFFICULT_MODE_H

#include <vector>
#include <algorithm>

#include "linked_list_table.h"
#include "game_scene.h"

#define EMPTY_BOARD -1

struct DifficultMode {
    int padding_left;
    int padding_top;
    int mode;

    int table_size;
    int remained_pairs;
    int cell_pos_x;
    int cell_pos_y;
    int cell_coord_x;
    int cell_coord_y;
    bool end_loop;
    std::vector<std::pair<int, int>> locked_list;
    TableLL* TableObject;
    GameScene *GameObject;

    DifficultMode(int, int, int);
    ~DifficultMode();

    void initTable();

    int getCellState(int, int) const;
    char getCellValue(int, int) const;
    void setCellState(int, int, int);
    void setCellValue(int, int, char);
    char getCharAt(int, int) const;

    void swapPoints(int &, int &);
    void swapCells(std::pair<int, int> &, std::pair<int, int> &);
    void selectCell(int, int, int);
    void unselectCell();
    void lockCell();
    void deleteCell();
    void displayCellAt(int, int, int, int);
    void cleanTableAtRow(int);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void startGame();

    void displayILine(std::pair<int, int>, std::pair<int, int>, bool);
    void displayLLine(std::pair<int, int>, std::pair<int, int>, bool);
    void displayZLine(std::pair<int, int>, std::pair<int, int>, bool);
    void displayBreakPointULine(std::pair<int, int>, std::pair<int, int>, std::pair<int, int>, std::pair<int, int>, bool);
    void displayVerticalULine(std::pair<int, int>, std::pair<int, int>, bool);
    void displayHorizontalULine(std::pair<int, int>, std::pair<int, int>, bool);

    bool isCharacterEqual(std::pair<int, int>, std::pair<int, int>);
    bool checkMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkIMatching(std::pair<int,int>, std::pair<int, int>);
    bool checkLMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkZMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkVerticalUMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkHorizontalUMatching(std::pair<int, int>, std::pair<int, int>);
    bool checkUMatching(std::pair<int, int>, std::pair<int, int>);
};

#endif // DIFFICULT_MODE_H
