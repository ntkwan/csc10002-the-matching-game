#include "menu_scene.h"

int main() {
    Screen::setUpConsole(18);
    Menu game;
    game.menuController();
    Sleep(5000);
    Screen::clearConsole();
}
