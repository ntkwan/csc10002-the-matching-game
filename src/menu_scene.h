#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <iostream>
#include <string>
#include <fstream>

#include "standard_mode.h"
#include "difficult_mode.h"

struct Menu {
    const std::string options[6] = {"Standard Mode", "Difficult Mode", "Challenge Mode", "Tutorial", "Leaderboard", "Exit"};
    const int option_slot = 6;

    std::string* game_title;
    std::string* menu_option;
    std::string* pikachu_asset;
    std::string* bulbasaur_asset;
    int current_option;
    int size_n;
    int size_m;

    Menu();
    ~Menu();

    bool inputTableSize(int);
    void playStandardMode();
    void playDifficultMode();
    void menuController();
    void loadMenuAssets(const std::string &, std::string *&);
    void displayMenuBackground();

    void changeOption(int);
    void selectOption();
    void unselectOption();
    void displayOptionText();
};
#endif // MENU_SCENE_H
