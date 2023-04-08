#ifndef IDENTIFY_SCENE_H
#define IDENTIFY_SCENE_H

#include <string>
#include <iostream>
#include <fstream>

#include "screen.h"
#include "player.h"

struct IdentifyMenu {
    const std::string options[3] = {"LOGIN", "REGISTER", "QUIT"};
    const int option_slot = 3;

    Player user;
    Player user_list[1000];
    int number_user = 0;

    void changeOption(int);
    void selectOption();
    void unselectOption();
    bool menuController(bool);
    void displayInformationBoard(const int, const int, const int, const int);
    void displayOptionText();
    void displayNotification(int, int, const std::string &, int);
    void displayMenuBackground(bool);

    bool loginMenu();
    void registerMenu();
    void loadUserData();
    void saveUserData();

    int current_option = 0;
};

#endif // IDENTIFY_SCENE_H
