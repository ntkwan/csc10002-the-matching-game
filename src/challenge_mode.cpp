#include "challenge_mode.h"

ChallengeMode::ChallengeMode(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top, Player _player, int _number_user, Player *_user_list) {
    padding_top = _padding_top;
    padding_left = padding_left;
    table_size_n = _table_size_n;
    table_size_m = _table_size_m;

    GameObject = new GameScene(0, 0, 0, 0);
    PlayerObject = _player;
    number_user = _number_user;
    user_list = _user_list;

    current_play.username = PlayerObject.username;
    current_play.password = PlayerObject.password;
    current_play.gamemode = "challenge";
    current_play.point = 0;
    current_play.lvl = 0;
}

std::pair<int, bool> ChallengeMode::startGame() {
    srand(time(NULL));
    table_size_n = rand() % 10;
    table_size_m = rand() % 10;
    int coin_flip = rand() % 2;

    auto validateInput = [](const int table_size, int _mode) {
        if (table_size <= 0 || table_size > (_mode == 0 ? 5 : 7)) return false;

        return true;
    };

    while (table_size_n * table_size_m < 2 || table_size_n * table_size_m > (coin_flip == 0 ? 20 : 42) ||
           (table_size_n * table_size_m) & 1 || validateInput(table_size_n, coin_flip) == false || validateInput(table_size_m, coin_flip) == false) {
        table_size_n = rand() % 10;
        table_size_m = rand() % 10;
    }

    bool is_mistaken = false;
    if (coin_flip == 0) {
        Screen::clearConsole();
        StandardMode game(table_size_n, table_size_m, 20, 3, current_play, number_user, user_list, CHALLENGE_MODE);
        std::pair<Player, bool> game_state = game.startGame();
        current_play.point = game_state.first.point;
        is_mistaken = game_state.second;
    } else {
        Screen::clearConsole();
        DifficultMode game(table_size_n, table_size_m, 20, 3, current_play, number_user, user_list, CHALLENGE_MODE);
        std::pair<Player, bool> game_state = game.startGame();
        current_play.point = game_state.first.point;
        is_mistaken = game_state.second;
    }

return std::make_pair(coin_flip, is_mistaken);
}

Player ChallengeMode::loopGame() {

    auto displayNotification = [](int _padding_left, int _padding_top, const std::string &text) {
        Screen::setConsoleColor(WHITE, GREEN);
        Screen::gotoXY(_padding_left, _padding_top);
        std::cout<<text;
    };

    bool in_game = true;
    while (in_game == true) {
        Screen::clearConsole();
        std::pair<int, bool> game_state = startGame();
        int game_mode = game_state.first;
        if (game_state.second == true) return current_play;
        displayNotification((game_mode == 0 ? 75 : 90), 16, "PRESS ANY KEYS TO NEXT PLAY OR ESC TO EXIT");
        ++current_play.lvl;
        char ch = getch();
        if (ch == 27) {
            GameObject->saveUserData(number_user, current_play);
            return current_play;
        }
    }
}
