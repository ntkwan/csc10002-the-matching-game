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
    int new_option = current_option + _direction;
    if (new_option < 0 || new_option >= option_slot) return;

    unselectOption();
    current_option = new_option;
    selectOption();
}

void Menu::selectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(WHITE, GREEN);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(175);

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(174);
}

void Menu::unselectOption() {
    int padding_left = 59, padding_top = 18;
    Screen::setConsoleColor(WHITE, BLACK);

    Screen::gotoXY(padding_left-3, padding_top+current_option*2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top+current_option*2);
    std::cout<<options[current_option];

    Screen::gotoXY(padding_left+21, padding_top+current_option*2);
    putchar(' ');
}

void Menu::playStandardMode() {
    Screen::clearConsole();
    StandardMode game(STANDARD_MODE, 20, 3);
    game.startGame();
}

void Menu::menuController() {
    displayMenuBackground();
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
                        playStandardMode();
                        break;
                }
        }
    }
}

void Menu::loadMenuAssets(const std::string &path, std::string *&assets) {
    std::fstream bg(path, std::fstream::in);

    for (int idx = 0; !bg.eof(); ++idx) {
        getline(bg, assets[idx]);
    }

    bg.close();
}

void Menu::displayMenuBackground() {
    Screen::setConsoleColor(WHITE, BLACK);
    Screen::clearConsole();
    loadMenuAssets("assets/game_title.txt", game_title);
    loadMenuAssets("assets/menu_option.txt", menu_option);
    loadMenuAssets("assets/pikachu.txt", pikachu_asset);
    loadMenuAssets("assets/bulbasaur.txt", bulbasaur_asset);

    int color[] = { LIGHT_AQUA, AQUA, LIGHT_BLUE, BLUE, LIGHT_PURPLE, PURPLE, BLUE};

	int loop = 14, colorCount = 0, padding_left = 33, padding_top = 1;
	while (loop--) {
		Screen::setConsoleColor(WHITE, color[colorCount % 7]);
        Screen::gotoXY(padding_left, padding_top);
		for (int i = 0; i < 12; ++i) {
            Screen::gotoXY(padding_left, padding_top + i);
            std::cout<<game_title[i]<<"\n";
		}
		Sleep(100);
		colorCount++;
	}

    Screen::setConsoleColor(WHITE, BLACK);
    padding_left = 50, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<menu_option[i]<<"\n";
    }

    Screen::setConsoleColor(WHITE, YELLOW);
    padding_left = 10, padding_top = 18;
    for (int i = 0;i < 13; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<pikachu_asset[i]<<"\n";
    }

    Screen::setConsoleColor(WHITE, AQUA);
    padding_left = 100, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<bulbasaur_asset[i]<<"\n";
    }
}

void Menu::displayOptionText() {
    Screen::setConsoleColor(WHITE, BLACK);
    int padding_left = 59, padding_top = 18;
    for (int i = 0;i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 2);
        std::cout<<options[i];
    }
    selectOption();
}
