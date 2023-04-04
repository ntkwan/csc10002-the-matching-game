#include "identify_scene.h"

int main() {
    Screen::setUpConsole(18);
    IdentifyMenu menu;
    menu.menuController();
    Screen::clearConsole();
}
