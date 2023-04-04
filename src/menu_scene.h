#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <iostream>
#include <string>
#include <fstream>

#include "standard_mode.h"
#include "difficult_mode.h"
#include "challenge_mode.h"
#include "identify_scene.h"
#include "player.h"

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

    IdentifyMenu IMenu;

    Menu();
    ~Menu();

    bool inputTableSize(int);
    void playStandardMode();
    void playDifficultMode();
    void playChallengeMode();
    void menuController();
    void loadMenuAssets(const std::string &, std::string *&);
    void displayMenuBackground(bool);
    void displayInformationBoard(const int, const int, const int, const int);

    void loadUserData();
    void loginMenu();
    void registerMenu();

    void changeOption(int);
    void selectOption();
    void unselectOption();
    void displayOptionText();
};
#endif // MENU_SCENE_H
