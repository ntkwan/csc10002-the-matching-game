#include "challenge_mode.h"

ChallengeMode::ChallengeMode(int _table_size_n, int _table_size_m, int _padding_left, int _padding_top)
             : padding_top(_padding_top),
               padding_left(_padding_left),
               table_size_n(_table_size_n),
               table_size_m(_table_size_m) {}

int ChallengeMode::startGame() {
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

    if (coin_flip == 0) {
        Screen::clearConsole();
        //StandardMode GameObject(table_size_n, table_size_m, padding_left, padding_top, IMenu.);
        //GameObject.startGame();
    } else {
        Screen::clearConsole();
        //DifficultMode GameObject(table_size_n, table_size_m, padding_left, padding_top);
        //GameObject.startGame();
    }

return coin_flip;
}

void ChallengeMode::loopGame() {

    auto displayNotification = [](int _padding_left, int _padding_top, const std::string &text, int delayed_time) {
        Screen::setConsoleColor(WHITE, GREEN);
        Screen::gotoXY(_padding_left, _padding_top);
        std::cout<<text;
        Sleep(delayed_time);
        Screen::gotoXY(_padding_left, _padding_top);
        for (size_t i = 0; i < text.size(); ++i) std::cout<<" ";
    };

    bool in_game = true;
    while (in_game) {
        Screen::clearConsole();
        int game_mode = startGame();
        displayNotification((game_mode == 0 ? 75 : 90), 16, "PRESS ANY KEYS TO NEXT PLAY OR ESC TO EXIT", 2000);
        if (Screen::getConsoleInput() == 1) {
            in_game = false;
            break;
        }
    }

}
