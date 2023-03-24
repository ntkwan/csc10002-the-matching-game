#include "screen.h"
#include "table.h"

int main() {
    Table board(4, 20, 3);
    board.generateTableData();
    board.printTableData();
}
