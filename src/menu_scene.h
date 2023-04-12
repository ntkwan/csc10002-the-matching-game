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

    bool inputTableSize(int);                   //create custom table size
    std::pair<Player, bool> playStandardMode(); //init and start playing standard mode
    Player playStandardMode(int, int, Player *, Table *); //has the same functionality as the above but for standard mode save files

    std::pair<Player, bool> playDifficultMode(); //init and start playing difficult mode
    Player playDifficultMode(int, int, Player *, TableLL *); //has the same functionality as the above but for difficult mode save files

    Player playChallengeMode(); //init and start playing challenge mode
    bool menuController(bool);  //handle menu screen
    void loadMenuAssets(const std::string &, std::string *&); //load menu assets to be displayed
    void displayMenuBackground(bool);  //display menu effects and game titles
    void displayInformationBoard(const int, const int, const int, const int); //display in-game user interface

    void loadUserData(); //import user data
    void loginMenu(); //display login screen
    void registerMenu(); //display register screen

    void changeOption(int); //change current option (use to iterate through options)
    void selectOption(); //select current option
    void unselectOption(); //unselect current option
    void displayOptionText(); //display menu options
};
#endif // MENU_SCENE_H
