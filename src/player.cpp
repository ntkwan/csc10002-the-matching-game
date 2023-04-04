#include "player.h"

Player::Player() {
    point = 0;
    lvl = 0;
    gamemode = "null";
}

Player::Player(const Player& _player) {
    username = _player.username;
    password = _player.password;
    gamemode = _player.gamemode;
    point = _player.point;
    lvl = _player.lvl;
}

