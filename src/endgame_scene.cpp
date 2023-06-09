#include "endgame_scene.h"

bool EndgameMenu::displayGameOverScreen(int pd_left, int pd_top, Player current_play) {
    Screen::setConsoleColor(BLACK, BLACK);
    Screen::clearConsole();
    Screen::setConsoleColor(BLACK, RED);
    Screen::playSound("audio/game_over.wav");
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

    loadTitle("assets/game_over.txt", title);
    for (int i = 0; i < 20; ++i) {
        Screen::gotoXY(pd_left, pd_top + i);
        std::cout<<title[i];
    }

    int padding_left = 66, padding_top = 18;
    Screen::setConsoleColor(BLACK, YELLOW);
    Screen::gotoXY(padding_left + 3, padding_top);
    std::cout<<"POINTS: "<<current_play.point;
    Screen::gotoXY(padding_left, padding_top + 1);
    std::cout<<"LEVELS PASSED: "<<current_play.lvl;

    Screen::setConsoleColor(BLACK, WHITE);
    padding_left = 66, padding_top = 22;
    for (int i = 0;i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 2);
        std::cout<<options[i];
    }

    selectOption();

    bool in_menu = true;
    while (in_menu) {
        switch(Screen::getConsoleInput()) {
            case 2:
                changeOption(-1);
                break;
            case 5:
                changeOption(1);
                break;
            case 6:
                in_menu = 0;
                switch(current_option) {
                    case 0:
                        Screen::playSound("audio/click.wav");
                        return true;
                    case 1:
                        Screen::playSound("audio/click.wav");
                        return false;
                }
        }
    }


    delete[] title;
    title = nullptr;

return false;
}

void EndgameMenu::changeOption(int _direction) {
    Screen::playSound("audio/menu_cursor.wav");
    int new_option = current_option + _direction;
    if (new_option < 0 || new_option >= option_slot) return;

    unselectOption();
    current_option = new_option;
    selectOption();
}

void EndgameMenu::selectOption() {
    int padding_left = 66, padding_top = 22;
    Screen::setConsoleColor(BLACK, GREEN);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(175);

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(174);
}

void EndgameMenu::unselectOption() {
    int padding_left = 66, padding_top = 22;
    Screen::setConsoleColor(BLACK, WHITE);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(' ');
}

