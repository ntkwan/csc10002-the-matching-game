#ifndef ENDGAME_SCENE_H
#define ENDGAME_SCENE_H

#include <string>
#include <fstream>
#include <iostream>

#include "screen.h"

struct EndgameMenu {
    const std::string options[2] = {"TRY AGAIN", "BACK TO MAIN MENU"};
    const int option_slot = 2;
    int current_option = 0;

    bool displayGameOverScreen(int, int);
    void unselectOption();
    void selectOption();
    void changeOption(int);
};
#endif // ENDGAME_SCENE_H
