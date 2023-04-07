#include "leaderboard_scene.h"

void LDBoardMenu::displayLDBoardScreen(int pd_left, int pd_top) {
    loadUserList();

    std::sort(user_list, user_list + number_user, [](Player a, Player b){
                return ((a.point > b.point) || ((a.point == b.point) && (a.lvl < b.lvl)));
         });

    Screen::setConsoleColor(BLACK, BLACK);
    Screen::clearConsole();
    Screen::setConsoleColor(BLACK, YELLOW);
    std::string *title = new std::string[20];
    auto loadTitle = [](const std::string &path, std::string *&title) {
        std::fstream in(path, std::fstream::in);
        int n = 0;
        for (int i = 0; !in.eof(); ++i, n = i)
        {
            getline(in, title[i]);
        }
        in.close();
        return n;
    };

    loadTitle("assets/leaderboard.txt", title);
    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(pd_left, pd_top + i);
        std::cout<<title[i];
        title[i] = ' ';
    }

    loadTitle("assets/crown.txt", title);
    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(pd_left - 15, pd_top + i + 2);
        std::cout<<title[i];
    }

    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(pd_left + 81, pd_top + i + 2);
        std::cout<<title[i];
    }


    Screen::gotoXY(pd_left + 1, pd_top + 6);
    for (int i = 0; i < 80; ++i) std::cout<<"-";

    Screen::setConsoleColor(BLACK, WHITE);
    int padding_left = 52, padding_top = 11;
    Screen::gotoXY(padding_left, padding_top);
    std::cout<<"NO.";
    Screen::gotoXY(padding_left + 6, padding_top);
    std::cout<<"USERNAME";
    Screen::gotoXY(padding_left + 30, padding_top);
    std::cout<<"POINT";
    Screen::gotoXY(padding_left + 40, padding_top);
    std::cout<<"LEVEL";
    Screen::gotoXY(padding_left, padding_top + 1);
    for (int i = 0; i < 45; ++i) std::cout<<"-";

    padding_left = 0, padding_top = 13;
    Screen::setConsoleColor(BLACK, LIGHT_YELLOW);
    loadTitle("assets/big_pikachu.txt", title);
    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<title[i];
    }

    padding_left = 90, padding_top = 13;
    Screen::setConsoleColor(BLACK, YELLOW);
    loadTitle("assets/diglett.txt", title);
    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<title[i];
    }

    padding_left = 52, padding_top = 13;
    for (int i = 0; i < 10; ++i) {
        if (i < number_user) {
            Screen::setConsoleColor(BLACK, WHITE);
            Screen::gotoXY(padding_left - 1, padding_top + i);
            std::cout<<"|";
            Screen::gotoXY(padding_left + 45, padding_top + i);
            std::cout<<"|";
        }

        if (i == 0) Screen::setConsoleColor(BLACK, LIGHT_YELLOW);
        else if (i == 1) Screen::setConsoleColor(BLACK, LIGHT_WHITE);
        else if (i == 2) Screen::setConsoleColor(BLACK, YELLOW);
        else Screen::setConsoleColor(BLACK, GRAY);

        if (i < number_user) {
            Screen::gotoXY(padding_left + 1, padding_top + i);
            std::cout<<i + 1;
            Screen::gotoXY(padding_left + 6, padding_top + i);
            std::cout<<user_list[i].username;
            Screen::gotoXY(padding_left + 30, padding_top + i);
            std::cout<<user_list[i].point;
            Screen::gotoXY(padding_left + 40, padding_top + i);
            std::cout<<user_list[i].lvl;
        }
    }

    if (number_user == 10) {
        Screen::setConsoleColor(BLACK, WHITE);
        Screen::gotoXY(52, 23);
        for (int i = 0; i < 45; ++i) std::cout<<"-";
    }

    while (getch() != 27);
}

void LDBoardMenu::loadUserList() {
    std::fstream in("user_data/database.txt", std::ios::in);

    Player *new_list = new Player[1000];
    std::string input_stream;
    Player dummy;
    int idx = 0;
    while (getline(in, input_stream, '/')) {
        dummy.username = input_stream;
        getline(in, input_stream, '/');
        dummy.password = input_stream;
        getline(in, input_stream, '/');
        dummy.gamemode = input_stream;
        getline(in, input_stream, '/');
        dummy.point = stoi(input_stream);
        getline(in, input_stream);
        dummy.lvl = stoi(input_stream);
        if (dummy.gamemode == "challenge") {
            new_list[idx++] = dummy;
        }
    }

    std::sort(new_list, new_list + idx, [](Player a, Player b) {
                return ((a.username < b.username) || ((a.username == b.username) && (a.point < b.point)) || (a.username == b.username) && (a.point == b.point) && (a.lvl > b.lvl));
         });


    number_user = 0;
    for (int i = 0; i+1 < idx; ++i) {
        if (new_list[i].username != new_list[i+1].username) {
            user_list[number_user++] = new_list[i];
        }
    }

    if (idx > 0) {
        user_list[number_user++] = new_list[idx-1];
    }

    delete[] new_list;
    in.close();
}
