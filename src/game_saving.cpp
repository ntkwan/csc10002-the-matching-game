#include "game_saving.h"

int SavingMenu::menuController() {
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
            case 1:
                return 2;
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
                        return 1;
                        break;
                    case 1:
                        Screen::playSound("audio/click.wav");
                        return 0;
                        break;
                }
        }
    }
}

void SavingMenu::saveGame(Player user, Table *game_state) {
    std::string path = "saves/std/" + user.username + ".bin";
    std::ofstream out(path, std::ios::binary);

    out.write(reinterpret_cast<char *> (&user.point), sizeof(user.point));
    out.write(reinterpret_cast<char *> (&user.lvl), sizeof(user.lvl));

    int table_size_n = game_state->table_size_n;
    int table_size_m = game_state->table_size_m;

    out.write(reinterpret_cast<char *> (&table_size_n), sizeof(table_size_n));
    out.write(reinterpret_cast<char *> (&table_size_m), sizeof(table_size_m));

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_state), sizeof(game_state->table_data[i][j].cell_state));
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_value), sizeof(game_state->table_data[i][j].cell_value));
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_x), sizeof(game_state->table_data[i][j].cell_coord_x));
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_y), sizeof(game_state->table_data[i][j].cell_coord_y));
        }
    }

    out.close();
}

void SavingMenu::saveGame(Player user, TableLL *game_state) {
    std::string path = "saves/dfclt/" + user.username + ".bin";
    std::ofstream out(path, std::ios::binary);

    out.write(reinterpret_cast<char *> (&user.point), sizeof(user.point));
    out.write(reinterpret_cast<char *> (&user.lvl), sizeof(user.lvl));

    int table_size_n = game_state->table_size_n;
    int table_size_m = game_state->table_size_m;

    out.write(reinterpret_cast<char *> (&table_size_n), sizeof(table_size_n));
    out.write(reinterpret_cast<char *> (&table_size_m), sizeof(table_size_m));

    for (int i = 0; i < table_size_m; ++i) {
        for (Cell* cur_node = game_state->table_data[i].head; cur_node != nullptr; cur_node = cur_node->next) {
            out.write(reinterpret_cast<char *> (&cur_node->cell_state), sizeof(cur_node->cell_state));
            out.write(reinterpret_cast<char *> (&cur_node->cell_value), sizeof(cur_node->cell_value));
            out.write(reinterpret_cast<char *> (&cur_node->cell_pos_x), sizeof(cur_node->cell_pos_x));
            out.write(reinterpret_cast<char *> (&cur_node->cell_pos_y), sizeof(cur_node->cell_pos_y));
            out.write(reinterpret_cast<char *> (&cur_node->cell_coord_x), sizeof(cur_node->cell_coord_x));
            out.write(reinterpret_cast<char *> (&cur_node->cell_coord_y), sizeof(cur_node->cell_coord_y));
        }
    }

    out.close();
}

void SavingMenu::loadGame(Player *&user, Table *&game_state) {
    std::string path = "saves/std/" + user->username + ".bin";
    std::ifstream in(path, std::ios::binary);

    in.read(reinterpret_cast<char *> (&user->point), 4);
    in.read(reinterpret_cast<char *> (&user->lvl), 4);

    int table_size_n = 0;
    int table_size_m = 0;
    in.read(reinterpret_cast<char *> (&table_size_n), 4);
    in.read(reinterpret_cast<char *> (&table_size_m), 4);

    game_state = new Table(table_size_n, table_size_m, 20, 3);

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_state), 4);
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_value), 1);
        }
    }

    for (int i = 0; i < table_size_n; ++i) {
        for (int j = 0; j < table_size_m; ++j) {
            game_state->table_data[i][j].cell_pos_x = i;
            game_state->table_data[i][j].cell_pos_y = j;
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_x), 4);
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_y), 4);
        }
    }

    in.close();
}

void SavingMenu::loadGame(Player *&user, TableLL *&game_state) {
    std::string path = "saves/dfclt/" + user->username + ".bin";
    std::ifstream in(path, std::ios::binary);

    in.read(reinterpret_cast<char *> (&user->point), 4);
    in.read(reinterpret_cast<char *> (&user->lvl), 4);

    int table_size_n = 0;
    int table_size_m = 0;
    in.read(reinterpret_cast<char *> (&table_size_n), 4);
    in.read(reinterpret_cast<char *> (&table_size_m), 4);

    game_state = new TableLL(table_size_n, table_size_m, 20, 3);

    for (int i = 0; i < table_size_m; ++i) {
        int cell_state = 0, cell_pos_x = 0, cell_pos_y = 0, cell_coord_x = 0, cell_coord_y = 0;
        char cell_value;
        for (int j = 0; j < table_size_n; ++j) {
            in.read(reinterpret_cast<char *> (&cell_state), 4);
            in.read(reinterpret_cast<char *> (&cell_value), 1);
            in.read(reinterpret_cast<char *> (&cell_pos_x), 4);
            in.read(reinterpret_cast<char *> (&cell_pos_y), 4);
            in.read(reinterpret_cast<char *> (&cell_coord_x), 4);
            in.read(reinterpret_cast<char *> (&cell_coord_y), 4);

            Cell* cur_node = new Cell(cell_value, cell_state, cell_coord_x, cell_coord_y, cell_pos_x, cell_pos_y);
            game_state->table_data[i].addTail(cur_node);
        }
    }

    in.close();
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

