#ifndef GAME_SAVING_H
#define GAME_SAVING_H

#include <string>
#include <iostream>
#include <fstream>

#include "screen.h"
#include "table.h"
#include "linked_list_table.h"
#include "player.h"

struct SavingMenu {
    const std::string options[2] = {"NEW GAME", "CONTINUE"};
    const int option_slot = 2;
    int current_option = 0;

    int menuController();
    void unselectOption();
    void selectOption();
    void changeOption(int);

    void saveGame(Player, Table *);
    void saveGame(Player, TableLL *);

    void loadGame(Player *&, Table *&);
    void loadGame(Player *&, TableLL *&);
};
#endif // GAME_SAVING_H
