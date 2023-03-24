#include "screen.h"
#include "table.h"

int main() {
    bool flag = false;
    while (flag == false || Screen::getConsoleInput() != 1) {
        Screen::setUpConsole(18);
        Table board(4, 50, 3);
        board.displayTableData();
        flag = true;
    }
    Screen::clearConsole();
}
