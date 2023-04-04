#ifndef IDENTIFY_SCENE_H
#define IDENTIFY_SCENE_H

#include <string>
#include <iostream>
#include <fstream>

#include "menu_scene.h"
#include "screen.h"
#include "player.h"

struct IdentifyMenu {
    const std::string options[3] = {"LOGIN", "REGISTER", "QUIT"};
    const int option_slot = 3;

    Player user;
    Player user_list[100];
    int number_user = 0;

    Menu menu;

    void changeOption(int);
    void selectOption();
    void unselectOption();
    void menuController();
    void displayInformationBoard(const int, const int, const int, const int);
    void displayOptionText();
    void displayNotification(int, int, const std::string &, int);

    bool loginMenu();
    void registerMenu();
    void loadUserData();
    void saveUserData();

    int current_option = 0;
};

#endif // IDENTIFY_SCENE_H
