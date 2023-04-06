#ifndef LEADERBOARD_SCENE_H
#define LEADERBOARD_SCENE_H

#include <iostream>
#include <fstream>
#include <algorithm>

#include "screen.h"
#include "player.h"

struct LDBoardMenu {
    int number_user;
    Player user_list[1000];

    void displayLDBoardScreen(int, int);
    void loadUserList();
};
#endif // LEADERBOARD_SCENE_H
