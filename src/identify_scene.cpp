#include "identify_scene.h"

void IdentifyMenu::changeOption(int _direction) {
    Screen::playSound("audio/menu_cursor.wav");
    int new_option = current_option + _direction;
    if (new_option < 0 || new_option >= option_slot) return;

    unselectOption();
    current_option = new_option;
    selectOption();
}

void IdentifyMenu::selectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(BLACK, GREEN);

    Screen::gotoXY(padding_left+3, padding_top+current_option*2);
    putchar(175);

    Screen::gotoXY(padding_left+6, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+17, padding_top+current_option*2);
    putchar(174);
}

void IdentifyMenu::displayMenuBackground(bool is_flash) {
    std::string *game_title = new std::string[13];
    Screen::setConsoleColor(BLACK, BLACK);
    Screen::clearConsole();

    auto loadMenuAssets = [](const std::string &path, std::string *assets) {
       std::fstream bg(path, std::ios::in);

        for (int idx = 0; !bg.eof(); ++idx) {
            getline(bg, assets[idx]);
        }

        bg.close();
    };

    loadMenuAssets("assets/game_title.txt", game_title);

    int color[] = { LIGHT_AQUA, AQUA, LIGHT_BLUE, BLUE, LIGHT_PURPLE, PURPLE, YELLOW};

	int loop = (is_flash == true ? 14 : 1), colorCount = (is_flash == true ? 0 : 6), padding_left = 33, padding_top = 1;
	while (loop--) {
		Screen::setConsoleColor(BLACK, color[colorCount % 7]);
        Screen::gotoXY(padding_left, padding_top);
		for (int i = 0; i < 12; ++i) {
            Screen::gotoXY(padding_left, padding_top + i);
            std::cout<<game_title[i]<<"\n";
		}
		Sleep(100);
		colorCount++;
	}

	delete[] game_title;
	game_title = nullptr;
}

void IdentifyMenu::unselectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(BLACK, WHITE);

    Screen::gotoXY(padding_left+3, padding_top+current_option*2);
    putchar(' ');

    Screen::gotoXY(padding_left+6, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+17, padding_top+current_option*2);
    putchar(' ');
}

void IdentifyMenu::displayInformationBoard(const int left, const int top, const int width, const int height) {
    Screen::setConsoleColor(BLACK, YELLOW);
    Screen::gotoXY(left, top);
    putchar('-');
    for (int i = 0; i < width; ++i) putchar('-');
    putchar('-');

    for (int i = 0; i < height; ++i)
    {
        Screen::gotoXY(left, top + i + 1);
        putchar(' ');
        Screen::gotoXY(left + width + 1, top + i + 1);
        putchar(' ');
        if (i == height-1)
        {
            Screen::gotoXY(left, top + i + 1);
            putchar('-');
        }
    }

    for (int i = 0; i < width; ++i) putchar('-');
    putchar('-');
}

void IdentifyMenu::displayOptionText() {
    Screen::setConsoleColor(BLACK, WHITE);
    int padding_left = 59, padding_top = 18;
    for (int i = 0;i < option_slot; ++i) {
        Screen::gotoXY(padding_left+6, padding_top + i * 2);
        std::cout<<options[i];
    }
    selectOption();
}

void IdentifyMenu::displayNotification(int _padding_left, int _padding_top, const std::string &text, int delayed_time) {
    Screen::setConsoleColor(BLACK, RED);
    Screen::gotoXY(_padding_left, _padding_top);
    std::cout<<text;
    Sleep(delayed_time);
    Screen::gotoXY(_padding_left, _padding_top);
    for (size_t i = 0; i < text.size(); ++i) std::cout<<" ";
}

bool IdentifyMenu::menuController() {
    displayMenuBackground(true);
    displayOptionText();

    auto validateAccountLogin = [](const std::string &username, const std::string &password, const int number_user, Player *user_list) {
        for (int i = 0; i < number_user; ++i) {
            if (username == user_list[i].username && password == user_list[i].password) {
                return true;
            }
        }

        return false;
    };

    auto validateAccountRegister = [](const std::string &username, const std::string &password, const int number_user, Player *user_list) {
        for (int i = 0; i < number_user; ++i) {
            if (username == user_list[i].username) {
                return false;
            }
        }

        return true;
    };


    bool in_menu = true;
    while (in_menu) {
        switch(Screen::getConsoleInput()) {
            case 1:
                Screen::clearConsole();
                break;
            case 2:
                changeOption(-1);
                break;
            case 5:
                changeOption(1);
                break;
            case 6:
                if (current_option == 0) {
                        Screen::clearConsole();
                        displayMenuBackground(false);
                        loadUserData();

                        while (true) {
                            loginMenu();
                            if (validateAccountLogin(user.username, user.password, number_user, user_list) == true) break;
                            else {
                                    displayNotification(56, 18, "WRONG USERNAME OR PASSWORD", 1000);
                            }
                        }
                        return true;
                } else if (current_option == 1) {
                        Screen::clearConsole();
                        displayMenuBackground(false);
                        loadUserData();
                        while (true) {
                            registerMenu();
                            if (validateAccountRegister(user.username, user.password, number_user, user_list) == true) {
                                    saveUserData();
                                    break;
                            }
                            else {
                                    displayNotification(56, 18, "THIS USERNAME WAS TAKEN", 1000);
                            }
                        }

                        Screen::clearConsole();
                        displayMenuBackground(false);
                        loadUserData();
                        while (true) {
                            loginMenu();
                            if (validateAccountLogin(user.username, user.password, number_user, user_list) == true) break;
                            else {
                                    displayNotification(56, 18, "WRONG USERNAME OR PASSWORD", 1000);
                            }
                        }
                        return true;
                } else {
                        in_menu = false;
                }
        }
    }
}

void IdentifyMenu::loadUserData() {
    std::fstream in("user_data/database.txt", std::ios::in);

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
        user_list[idx++] = dummy;
    }

    number_user = idx;
    in.close();
}

void IdentifyMenu::saveUserData() {
    std::fstream out("user_data/database.txt", std::ios::app);

    if (number_user > 0) out<<"\n";
    out<<user.username<<"/"<<user.password<<"/"<<user.gamemode<<"/"<<user.point<<"/"<<user.lvl;

    out.close();
}

bool IdentifyMenu::loginMenu() {
    displayInformationBoard(56, 20, 25, 7);
    Screen::gotoXY(45, 18);
    std::cout<<"USERNAME AND PASSWORD MUST BE LESS THAN 10 CHARACTERS";
    Screen::gotoXY(55, 19);
    std::cout<<"AND NO BACKSPACES IN THE BACK";
    Screen::gotoXY(58, 22);
    std::cout<<"USERNAME: ";
    Screen::gotoXY(58, 25);
    std::cout<<"PASSWORD: ";

    Screen::gotoXY(68, 22);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(68, 25);
    for (int i = 0; i < 100; ++i) std::cout<<" ";

    auto validateInput = [](const std::string &text) {
        if (text.size() == 0 || text.size() > 10 || text.back() == ' ') return false;

        return true;
    };

    Screen::gotoXY(68, 22);
    Screen::showCursor(true);
    user.username.clear();
    getline(std::cin, user.username);
    while (validateInput(user.username) == false) {
        Screen::gotoXY(68, 22);
        for (int i = 0;i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(68, 22);
        getline(std::cin, user.username);
    }


    auto displayPasswordField = [](char *psw) {
        for (int i = 0; true; ) {
            char ch = getch();

            if (ch == 13) {
                Screen::showCursor(false);
                psw[i] = '\0';
                break;
            }
            else if (ch == 8) {
                if (i > 0) {
                    --i;
                    printf("\b \b");
                }
            }
            else {
                psw[i] = ch;
                ++i;
                printf("* \b");
            }
        }
    };


    Screen::gotoXY(68, 25);
    user.password.clear();
    while (validateInput(user.password) == false) {
        char psw[10];
        Screen::gotoXY(68, 25);
        for (int i = 0; i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(68, 25);
        displayPasswordField(psw);

        std::string password(psw);
        user.password = password;
    }

    Screen::gotoXY(40, 18);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(40, 19);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
}

void IdentifyMenu::registerMenu() {
    displayInformationBoard(50, 20, 33, 10);
    Screen::gotoXY(40, 18);
    std::cout<<"USERNAME AND PASSWORD MUST BE LESS THAN 10 CHARACTERS";
    Screen::gotoXY(53, 19);
    std::cout<<"AND NO BACKSPACES IN THE BACK";
    Screen::gotoXY(50, 22);
    std::cout<<"USERNAME: ";
    Screen::gotoXY(50, 25);
    std::cout<<"PASSWORD: ";
    Screen::gotoXY(50, 28);
    std::cout<<"CONFIRM PASSWORD: ";

    Screen::gotoXY(65, 22);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(65, 25);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(67, 28);
    for (int i = 0; i < 100; ++i) std::cout<<" ";

    auto validateInput = [](const std::string &text) {
        if (text.size() == 0 || text.size() > 10 || text.back() == ' ') return false;

        return true;
    };

    Screen::gotoXY(68, 22);
    Screen::showCursor(true);
    user.username.clear();
    getline(std::cin, user.username);
    while (validateInput(user.username) == false) {
        Screen::gotoXY(68, 22);
        for (int i = 0;i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(68, 22);
        getline(std::cin, user.username);
    }

    auto displayPasswordField = [](char *psw) {
        for (int i = 0; true; ) {
            char ch = getch();

            if (ch == 13) {
                psw[i] = '\0';
                break;
            }
            else if (ch == 8) {
                if (i > 0) {
                    --i;
                    printf("\b \b");
                }
            }
            else {
                psw[i] = ch;
                ++i;
                printf("* \b");
            }
        }
    };

    user.password.clear();
    std::string dummy_password = "#";

    while (user.password != dummy_password) {
        user.password.clear();
        dummy_password = "#";

        Screen::showCursor(true);
        Screen::gotoXY(68, 25);
        while (validateInput(user.password) == false) {
            char psw[10];
            Screen::gotoXY(68, 25);
            for (int i = 0; i < 100; ++i) std::cout<<" ";
            Screen::gotoXY(68, 25);
            displayPasswordField(psw);

            std::string password(psw);
            user.password = password;
        }

        Screen::showCursor(true);
        Screen::gotoXY(68, 28);
        while (validateInput(dummy_password) == false || dummy_password[0] == '#') {
            char psw[10];
            Screen::gotoXY(68, 28);
            for (int i = 0; i < 100; ++i) std::cout<<" ";
            Screen::gotoXY(68, 28);
            displayPasswordField(psw);

            std::string password(psw);
            dummy_password = password;
        }

        Screen::gotoXY(68, 25);
        for (int i = 0; i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(68, 28);
        for (int i = 0; i < 100; ++i) std::cout<<" ";
    }

    Screen::showCursor(false);
    Screen::gotoXY(40, 18);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(40, 19);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
    Screen::gotoXY(68, 22);
    for (int i = 0; i < 100; ++i) std::cout<<" ";
}
