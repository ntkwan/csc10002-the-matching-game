#ifndef CHALLENGE_MODE_H
#define CHALLENGE_MODE_H

#include "standard_mode.h"
#include "difficult_mode.h"
#include "menu_scene.h"

struct ChallengeMode {
        int table_size_n;           //to generate a random number for table's row size
        int table_size_m;           //to generate a random number for table's column size

        GameScene *GameObject;      //to save user data when game ends
        Player *user_list;          //to maintain user list to other scenes
        int number_user;            //to maintain number of users to other scenes
        Player current_play;        //to receive current playing information from previous scene to other scenes
        const int mistake = 3;      //each level of challenge mode will be 3 mistakes fixed

        ChallengeMode(int, int, Player, int, Player *); //structure constructor

        std::pair<int, bool> startGame();   //to generate a random number for table's size and randomize difficulty stage
        Player loopGame();                  //to loop the game until the player making mistakes for 3 times or stop playing by pressing ESC button
};

#endif // CHALLENGE_MODE_H
