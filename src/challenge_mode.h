#ifndef CHALLENGE_MODE_H
#define CHALLENGE_MODE_H

#include "standard_mode.h"
#include "difficult_mode.h"
#include "menu_scene.h"

struct ChallengeMode {
        int padding_left = 0;
        int padding_top = 0;

        int table_size_n;
        int table_size_m;

        ChallengeMode(int, int, int, int);

        int startGame();
        void loopGame();
};


#endif // CHALLENGE_MODE_H
