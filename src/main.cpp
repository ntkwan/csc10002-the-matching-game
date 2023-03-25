#include "game.h"

int main() {
    Screen::setUpConsole(18);
    Game game;
    game.startGame();
    Screen::clearConsole();
}
