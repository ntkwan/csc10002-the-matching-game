#include "game_saving.h"

bool SavingMenu::menuController() {
    Screen::setConsoleColor(BLACK, WHITE);
    int padding_left = 65, padding_top = 20;
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
                        break;
                    case 1:
                        Screen::playSound("audio/click.wav");
                        return false;
                        break;
                }
        }
    }
}

void SavingMenu::saveGame(Player user, Table *game_state) {
    std::string path = "saves/std/" + user.username + ".txt";
    std::ofstream out(path);

    out<<user.point<<" "<<user.lvl<<"\n";

    int table_size_n = game_state->table_size_n;
    int table_size_m = game_state->table_size_m;
    out<<game_state->table_size_n<<" "<<game_state->table_size_m<<"\n";
    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out<<game_state->table_data[i][j].cell_state<<" ";
        }
        out<<"\n";
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out<<game_state->table_data[i][j].cell_value<<" ";
        }
        out<<"\n";
    }


    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out<<game_state->table_data[i][j].cell_coord_x<<" "<<game_state->table_data[i][j].cell_coord_y<<"\n";
        }
    }

    out.close();
}

void SavingMenu::loadGame(Player *&user, Table *&game_state) {
    std::string path = "saves/std/" + user->username + ".txt";
    std::ifstream in(path);

    in>>user->point>>user->lvl;

    int table_size_n = 0;
    int table_size_m = 0;
    in>>table_size_n>>table_size_m;

    game_state = new Table(table_size_n, table_size_m, 20, 3);

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            in>>game_state->table_data[i][j].cell_state;
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            in>>game_state->table_data[i][j].cell_value;
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            game_state->table_data[i][j].cell_pos_x = i;
            game_state->table_data[i][j].cell_pos_y = j;
            in>>game_state->table_data[i][j].cell_coord_x>>game_state->table_data[i][j].cell_coord_y;
        }
    }
}

void SavingMenu::changeOption(int _direction) {
    Screen::playSound("audio/menu_cursor.wav");
    int new_option = current_option + _direction;
    if (new_option < 0 || new_option >= option_slot) return;

    unselectOption();
    current_option = new_option;
    selectOption();
}

void SavingMenu::selectOption() {
    int padding_left = 65, padding_top = 20;
    Screen::setConsoleColor(BLACK, GREEN);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(175);

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+10, padding_top+current_option*2);
    putchar(174);
}

void SavingMenu::unselectOption() {
    int padding_left = 65, padding_top = 20;
    Screen::setConsoleColor(BLACK, WHITE);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+10, padding_top+current_option*2);
    putchar(' ');
}

