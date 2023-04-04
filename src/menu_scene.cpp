#include "menu_scene.h"

Menu::Menu() {
    current_option = 0;
    game_title = new std::string[30];
    menu_option = new std::string[30];
    pikachu_asset = new std::string[36];
    bulbasaur_asset = new std::string[36];
}

Menu::~Menu() {
    delete[] game_title;
    game_title = nullptr;

    delete[] menu_option;
    menu_option = nullptr;

    delete[] pikachu_asset;
    pikachu_asset = nullptr;

    delete[] bulbasaur_asset;
    bulbasaur_asset = nullptr;
}

void Menu::changeOption(int _direction) {
    Screen::playSound("audio/menu_cursor.wav");
    int new_option = current_option + _direction;
    if (new_option < 0 || new_option >= option_slot) return;

    unselectOption();
    current_option = new_option;
    selectOption();
}

void Menu::selectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(BLACK, GREEN);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(175);

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(174);
}

void Menu::unselectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(BLACK, WHITE);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(' ');
}

bool Menu::inputTableSize(int _mode) {
    Screen::clearConsole();

    displayInformationBoard(56, 3, 25, 10);
    Screen::gotoXY(63, 4);
    std::cout<<"- TABLE SIZE -";
    Screen::gotoXY(60, 6);
    std::cout<<"ROWS: ";
    Screen::gotoXY(60, 8);
    std::cout<<"COLUMNS: ";
    Screen::gotoXY(59, 11);
    std::cout<<"INPUT A VALID NUMBER";
    Screen::gotoXY(62, 12);
    std::cout<<"BETWEEN 1 AND "<<(_mode == STANDARD_MODE ? 5 : 7);

    auto validateInput = [](const std::string &table_size, int _mode) {
        if (table_size.size() == 0 || table_size.size() > 1) return false;

        for (size_t i = 0; i < table_size.size(); ++i) {
            if (table_size[i] < '0' || table_size[i] > (_mode == STANDARD_MODE ? '5' : '7')) return false;
        }

        return true;
    };

    Screen::showCursor(true);
    std::string user_size_n;
    Screen::gotoXY(73, 6);
    getline(std::cin, user_size_n);
    while (validateInput(user_size_n, _mode) == false) {
        Screen::gotoXY(73, 6);
        for (int i = 0; i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(73, 6);
        getline(std::cin, user_size_n);
    }
    size_n = stoi(user_size_n);

    std::string user_size_m;
    Screen::gotoXY(73, 8);
    getline(std::cin, user_size_m);
    while (validateInput(user_size_m, _mode) == false) {
        Screen::gotoXY(73, 8);
        for (int i = 0; i < 100; ++i) std::cout<<" ";
        Screen::gotoXY(73, 8);
        getline(std::cin, user_size_m);
    }
    size_m = stoi(user_size_m);

    if (size_n * size_m > _mode || size_n * size_m == 0 || (size_n * size_m) & 1) {
        Screen::setConsoleColor(BLACK, RED);
        Screen::gotoXY(56, 10);
        std::cout<<"THE TABLE CELLS MUST BE EVEN";
        Screen::gotoXY(59, 11);
        std::cout<<"AND BETWEEN 2 AND "<<(_mode == STANDARD_MODE ? 20 : 42);
        Screen::gotoXY(62, 12);
        std::cout<<"               ";
        Screen::gotoXY(79, 11);
        Sleep(2000);
        return false;
    }

    Screen::gotoXY(61, 14);
    Screen::setConsoleColor(BLACK, GREEN);
    std::cout<<"BOARD GENERATING";
    for (int i = 0; i < 3; ++i) {
        Sleep(500);
        std::cout<<".";
    }
    Sleep(500);
    std::swap(size_n, size_m);
return true;
}

void Menu::displayInformationBoard(const int left, const int top, const int width, const int height) {
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

void Menu::playStandardMode() {
    Screen::clearConsole();
    StandardMode game(size_n, size_m, 20, 3, IMenu.user, IMenu.number_user, IMenu.user_list);
    game.startGame();
}

void Menu::playDifficultMode() {
    Screen::clearConsole();
    DifficultMode game(size_n, size_m, 20, 3);
    game.startGame();
}

void Menu::playChallengeMode() {
    ChallengeMode game(0, 0, 20, 3);
    game.loopGame();
}

void Menu::menuController() {
    while (IMenu.menuController() == false);

    Screen::playSound("audio/menu_sound.wav");
    displayMenuBackground(true);
    displayOptionText();

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
                        while (inputTableSize(STANDARD_MODE) == false);
                        Screen::showCursor(false);
                        playStandardMode();
                        break;
                    case 1:
                        Screen::playSound("audio/click.wav");
                        while (inputTableSize(DIFFICULT_MODE) == false);
                        Screen::showCursor(false);
                        playDifficultMode();
                        break;
                    case 2:
                        Screen::playSound("audio/click.wav");
                        Screen::showCursor(false);
                        playChallengeMode();
                        break;
                }
        }
    }
}

void Menu::loadMenuAssets(const std::string &path, std::string *&assets) {
    std::fstream bg(path, std::ios::in);

    for (int idx = 0; !bg.eof(); ++idx) {
        getline(bg, assets[idx]);
    }

    bg.close();
}

void Menu::displayMenuBackground(bool is_flash) {
    Screen::setConsoleColor(BLACK, BLACK);
    Screen::clearConsole();
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
}

void Menu::displayOptionText() {
    loadMenuAssets("assets/pikachu.txt", pikachu_asset);
    loadMenuAssets("assets/bulbasaur.txt", bulbasaur_asset);
    loadMenuAssets("assets/menu_option.txt", menu_option);
    Screen::setConsoleColor(BLACK, WHITE);
    int padding_left = 50, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<menu_option[i]<<"\n";
    }

    Screen::setConsoleColor(BLACK, YELLOW);
    padding_left = 10, padding_top = 18;
    for (int i = 0;i < 13; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<pikachu_asset[i]<<"\n";
    }

    Screen::setConsoleColor(BLACK, YELLOW);
    padding_left = 100, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<bulbasaur_asset[i]<<"\n";
    }

    Screen::setConsoleColor(BLACK, WHITE);
    padding_left = 59, padding_top = 18;
    for (int i = 0;i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 2);
        std::cout<<options[i];
    }
    selectOption();
}
