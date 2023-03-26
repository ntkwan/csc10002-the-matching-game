#ifndef CELL_H
#define CELL_H

#define FREE 0
#define LOCKED 1
#define DELETED 2

struct Cell {
    char cell_value;
    int cell_state;
    int cell_coord_x;
    int cell_coord_y;

    Cell();
    Cell(Cell* _ptr);

    char getCellValue() const;
    int getCellState() const;
    int getCellCoordX() const;
    int getCellCoordY() const;

    void setCellValue(char _cell_value);
    void setCellState(int _cell_state);
    void setCellCoordX(int _cell_coord_x);
    void setCellCoordY(int _cell_coord_y);
};

#endif // CELL_H
