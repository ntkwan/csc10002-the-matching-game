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
    Cell *next;
    Cell *prev;

    Cell();
    Cell(Cell*);
    Cell(char, char, int, int, int, int);

    char getCellValue() const;
    int getCellState() const;
    int getCellCoordX() const;
    int getCellCoordY() const;
    int getCellPosX() const;
    int getCellPosY() const;

    void setCellValue(char);
    void setCellState(int);
    void setCellCoordX(int);
    void setCellCoordY(int);
    void setCellPosX(int);
    void setCellPosY(int);
};

#endif // CELL_H
