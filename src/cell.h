#ifndef CELL_H
#define CELL_H

struct Cell {
    char cell_value;
    int cell_state;
    int cell_coord_x;
    int cell_coord_y;
    int cell_pos_x;
    int cell_pos_y;

    Cell();

    char getCellValue() const;
    int getCellState() const;
    int getCellCoordX() const;
    int getCellCoordY() const;
    int getCellPosX() const;
    int getCellPosY() const;

    void setCellValue(char _cell_value);
    void setCellState(int _cell_state);
    void setCellCoordX(int _cell_coord_x);
    void setCellCoordY(int _cell_coord_y);
    void setCellPosX(int _cell_pos_x);
    void setCellPosY(int _cell_pos_y);
};

#endif // CELL_H
