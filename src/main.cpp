#include "menu_scene.h"

int main() {
    Screen::setUpConsole(18);
    Menu menu;
    menu.menuController();
    Screen::clearConsole();
}
