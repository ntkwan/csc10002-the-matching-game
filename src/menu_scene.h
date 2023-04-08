#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <iostream>
#include <string>
#include <fstream>

#include "standard_mode.h"
#include "difficult_mode.h"
#include "challenge_mode.h"
#include "identify_scene.h"
#include "endgame_scene.h"
#include "game_saving.h"
#include "leaderboard_scene.h"
#include "tutorial_scene.h"
#include "player.h"

struct Menu {
    const std::string options[7] = {"Standard Mode", "Difficult Mode", "Challenge Mode", "Tutorial", "Leaderboard", "Logout", "Exit"};
    const int option_slot = 7;

    std::string* game_title;
    std::string* menu_option;
    std::string* pikachu_asset;
    std::string* bulbasaur_asset;
    int current_option;
    int size_n;
    int size_m;

    IdentifyMenu IMenu;
    EndgameMenu EMenu;
    LDBoardMenu LMenu;
    SavingMenu SMenu;
    TutorialMenu TMenu;

    Menu();
    ~Menu();

    bool inputTableSize(int);
    std::pair<Player, bool> playStandardMode();
    Player playStandardMode(int, int, Player *, Table *);

    std::pair<Player, bool> playDifficultMode();
    Player playDifficultMode(int, int, Player *, TableLL *);

    Player playChallengeMode();
    bool menuController(bool);
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
