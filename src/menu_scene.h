#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <iostream>
#include <string>
#include <fstream>

#include "game.h"

struct Menu {

    std::string* game_title;
    std::string* menu_option;
    std::string* pikachu_asset;
    std::string* bulbasaur_asset;

    Menu();

    void loadMenuAssets(const std::string &, std::string *&);
    void displayMenu();
};
#endif // MENU_SCENE_H
