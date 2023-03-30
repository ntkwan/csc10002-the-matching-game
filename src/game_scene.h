#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <stdio.h>
#include <string>
#include <fstream>

#include "screen.h"
#include "cell.h"

struct GameScene {
    int table_size;
    int padding_left;
    int padding_top;
    std::string* table_image;

    GameScene(int, int, int);
    ~GameScene();

    int getXInConsole(int);
    int getYInConsole(int);


    void cleanMatchingEffect();
    void loadTableBackground(const std::string &);
    void displayTableBorder();
    void displayTableBackground();
};

#endif // GAME_SCENE_H
