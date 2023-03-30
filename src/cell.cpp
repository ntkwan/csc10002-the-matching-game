#include "cell.h"

Cell::Cell()
    : cell_value(' '),
      cell_state(0),
      cell_coord_x(0),
      cell_coord_y(0),
      cell_pos_x(0),
      cell_pos_y(0),
      next(nullptr),
      prev(nullptr) {}

Cell::Cell(Cell* _ptr)
    : cell_value(_ptr->cell_value),
      cell_state(_ptr->cell_state),
      cell_coord_x(_ptr->cell_coord_x),
      cell_coord_y(_ptr->cell_coord_y),
      cell_pos_x(_ptr->cell_pos_x),
      cell_pos_y(_ptr->cell_pos_y),
      next(_ptr->next),
      prev(_ptr->prev) {}

Cell::Cell(char _cell_value, char _cell_state, int _cell_coord_x, int _cell_coord_y, int _cell_pos_x, int _cell_pos_y)
    : cell_value(_cell_value),
      cell_state(_cell_state),
      cell_coord_x(_cell_coord_x),
      cell_coord_y(_cell_coord_y),
      cell_pos_x(_cell_pos_x),
      cell_pos_y(_cell_pos_y),
      next(nullptr),
      prev(nullptr) {}

char Cell::getCellValue() const { return cell_value; }

int Cell::getCellState() const { return cell_state; }

int Cell::getCellCoordX() const { return cell_coord_x; }

int Cell::getCellCoordY() const { return cell_coord_y; }

int Cell::getCellPosX() const { return cell_pos_x; }

int Cell::getCellPosY() const { return cell_pos_y; }

void Cell::setCellValue(char _cell_value) { cell_value = _cell_value; }

void Cell::setCellState(int _cell_state) { cell_state = _cell_state; }

void Cell::setCellCoordX(int _cell_coord_x) { cell_coord_x = _cell_coord_x; }

void Cell::setCellCoordY(int _cell_coord_y) { cell_coord_y = _cell_coord_y; }

void Cell::setCellPosX(int _cell_pos_x) { cell_pos_x = _cell_pos_x; }

void Cell::setCellPosY(int _cell_pos_y) { cell_pos_y = _cell_pos_y; }
