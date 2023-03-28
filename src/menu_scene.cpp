#include "menu_scene.h"

Menu::Menu() {
    game_title = new std::string[30];
    menu_option = new std::string[30];
    pikachu_asset = new std::string[36];
    bulbasaur_asset = new std::string[36];
}

void Menu::loadMenuAssets(const std::string &path, std::string *&assets) {
    std::fstream bg(path, std::fstream::in);

    for (int idx = 0; !bg.eof(); ++idx) {
        getline(bg, assets[idx]);
    }

    bg.close();
}

void Menu::displayMenu() {
    Screen::setConsoleColor(WHITE, BLACK);
    Screen::clearConsole();
    loadMenuAssets("assets/game_title.txt", game_title);
    loadMenuAssets("assets/menu_option.txt", menu_option);
    loadMenuAssets("assets/pikachu.txt", pikachu_asset);
    loadMenuAssets("assets/bulbasaur.txt", bulbasaur_asset);

    int color[] = { LIGHT_AQUA, AQUA, LIGHT_BLUE, BLUE, LIGHT_PURPLE, PURPLE, BLACK};

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

    padding_left = 50, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<menu_option[i]<<"\n";
    }

    padding_left = 10, padding_top = 18;
    for (int i = 0;i < 13; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<pikachu_asset[i]<<"\n";
    }

    padding_left = 100, padding_top = 15;
    for (int i = 0;i < 17; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout<<bulbasaur_asset[i]<<"\n";
    }
}
