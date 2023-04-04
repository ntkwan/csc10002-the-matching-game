#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
    std::string username;
    std::string password;

    std::string gamemode = "null";
    int point = 0;
    int lvl = 0;

    Player();
    Player(const Player &);
};
#endif // PLAYER_H
