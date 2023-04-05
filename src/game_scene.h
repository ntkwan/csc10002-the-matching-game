#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

#include "screen.h"
#include "cell.h"
#include "player.h"

struct GameScene {
    int table_size_n;
    int table_size_m;
    int padding_left;
    int padding_top;
    std::string* table_image;

    GameScene(int, int, int, int);
    ~GameScene();

    int getXInConsole(int);
    int getYInConsole(int);


    void cleanMatchingEffect();
    void loadTableBackground(const std::string &);
    void displayTableBorder();
    void displayTableBackground();
    void displayUserInterface(int, int, int);
    void displayInfomationBoard(int, int, int, int);

    void displayNotification(int, int, const std::string &, int);
    void displayNotification(int, int, const std::string &);

    void displayUserAttributes(int, int, Player *, Player, int);
    void loadUserData(int, int, Player *, Player *&);
    void saveUserData(int, Player);
};

#endif // GAME_SCENE_H
