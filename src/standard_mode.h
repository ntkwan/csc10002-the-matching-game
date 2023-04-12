#ifndef STANDARD_MODE_H
#define STANDARD_MODE_H

#include <vector>
#include <algorithm>

#include "table.h"
#include "game_scene.h"
#include "game_saving.h"

struct StandardMode {
    int padding_left = 0; //table's left padding
    int padding_top = 0;  //table's top padding
    int mode;             //game mode
    int table_size_n;     //table row size
    int table_size_m;     //table column size
    int remained_pairs;   //table pairs remaining
    int cell_pos_x;       //current cursor cell's x position on the table
    int cell_pos_y;       //current cursor cell's y position on the table
    int cell_coord_x;     //current cursor cell's x coordination on the console
    int cell_coord_y;     //current cursor cell's y coordination on the console
    bool end_loop;        //handle game looping
    bool is_loaded;       //to check if the game's state is from loaded file or not
    std::vector<std::pair<int, int>> locked_list; //selected cells list
    Table *TableObject;    //table data structure
    GameScene *GameObject; //handle in-game graphics

    SavingMenu SMenu;      //handle game saving

    Player *PlayerObject;  //handle player account
    Player *user_list;     //handle player's attributes to display on the leaderboard
    int number_user;       //number users on this local PC
    Player current_play;   //current game's state
    int mistake;           //mistakes remianing

    StandardMode(int, int, int, int, Player, int, Player *, Player *, Table *, int); //game mode constructor
    ~StandardMode();       //game mode destructor

    void initTable();                   //init table

    int getCellState(int, int) const;    //get the state which cell contains (there are 4 states: FREE, LOCKED, DELETED, EMPTY BOARD)
    char getCellValue(int, int) const;   //get the character which cell contains
    void setCellState(int, int, int);    //set the cell's state
    void setCellValue(int, int, char);   //set the cell's value
    char getCharAt(int, int) const;      //get the cell's character (the DELETED character will return blank space)
    void displayCellValueAt(int, int, int, int); //display the cell's character (the DELETED character will be skipped)

    template<class T> void swapPoints(T &, T &); //swap two cells
    void selectCell(int, int, int);              //select a cell (point the cursor to that cell)
    void unselectCell();                         //unselect the current cell
    void lockCell();                             //selected the current cell
    void deleteCell();                           //after found a pair matched, we will delete those cells from the table

    void moveUp();                               //move the cursor up
    void moveDown();                             //move the cursor down
    void moveLeft();                             //move the cursor left
    void moveRight();                            //move the cursor right

    std::pair<Player, bool> startGame();         //init the game mode

    void displayTableData();                     //display table data

    void displayILine(std::pair<int, int>, std::pair<int, int>, bool); //display I matching effects
    void displayLLine(std::pair<int, int>, std::pair<int, int>, bool); //display L matching effects
    void displayZLine(std::pair<int, int>, std::pair<int, int>, bool); //display Z matching effects
    void displayBreakPointULine(std::pair<int, int>, std::pair<int, int>, std::pair<int, int>, std::pair<int, int>, bool); //display corner cell appearance
    void displayVerticalULine(std::pair<int, int>, std::pair<int, int>, bool); //display vertical U matching effects
    void displayHorizontalULine(std::pair<int, int>, std::pair<int, int>, bool);  //display horizontal U matching effects

    bool isCharacterEqual(std::pair<int, int>, std::pair<int, int>); //check two cells have the same character
    bool checkMatching(std::pair<int, int>, std::pair<int, int>, bool); //checker handles all cases
    bool checkIMatching(std::pair<int,int>, std::pair<int, int>); //check I matching pattern
    bool checkLMatching(std::pair<int, int>, std::pair<int, int>); //check L matching pattern
    bool checkZMatching(std::pair<int, int>, std::pair<int, int>); //check Z matching pattern
    bool checkVerticalUMatching(std::pair<int, int>, std::pair<int, int>); //check vertical U matching pattern
    bool checkHorizontalUMatching(std::pair<int, int>, std::pair<int, int>); //check horizontal U matching pattern
    bool checkUMatching(std::pair<int, int>, std::pair<int, int>, bool); //check U matching pattern

    bool findValidPairs(bool); //move suggestion - find valid pairs
};

#endif // STANDARD_MODE_H
