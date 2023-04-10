#ifndef CELL_H
#define CELL_H

#define FREE 0
#define LOCKED 1
#define DELETED 2
#define EMPTY_BOARD -1

#define STANDARD_MODE 20
#define DIFFICULT_MODE 42
#define CHALLENGE_MODE 100

const int CELL_LENGTH = 8;
const int CELL_HEIGHT = 4;

struct Cell {
    char cell_value;
    int cell_state;
    int cell_pos_x;
    int cell_pos_y;
    int cell_coord_x;
    int cell_coord_y;
    Cell *next;                             //the next cell on linked list
    Cell *prev;                             //the previous cell on linked list

    Cell();                                 //structure constructor
    Cell(Cell*);                            //assign another cell to the current cell
    Cell(char, char, int, int, int, int);   //assign cell elements

    char getCellValue() const;              //get the character which cell contains
    int getCellState() const;               //get the state which cell contains (there are 4 states: FREE, LOCKED, DELETED, EMPTY BOARD)
    int getCellCoordX() const;              //get the cell's x coordination for displaying
    int getCellCoordY() const;              //get the cell's y coordination for displaying
    int getCellPosX() const;                //get the cell's row position on 2D dynamically allocated array
    int getCellPosY() const;                //get the cell's column position  on 2D dynamically allocated array

    void setCellValue(char);                //assign new character to the cell
    void setCellState(int);                 //assign new state to the cell
    void setCellCoordX(int);                //assign new cell's x coordination
    void setCellCoordY(int);                //assign new cell's y coordination
    void setCellPosX(int);                  //assign new cell's row position
    void setCellPosY(int);                  //assign new cell's column position
};

#endif // CELL_H
