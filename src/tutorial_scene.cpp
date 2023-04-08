#include "tutorial_scene.h"

void TutorialMenu::displayGameTutorial() {
    Screen::setConsoleColor(BLACK, BLACK);
    Screen::clearConsole();

    auto printTitle = [](const int pd_left, const int pd_top, const std::string &text) {
        Screen::gotoXY(pd_left, pd_top);
        std::cout<<text;
    };

    auto printText = [](const int _background, const int _color, const std::string &text) {
        Screen::setConsoleColor(_background, _color);
        std::cout<<text;
    };

    Screen::setConsoleColor(BLACK, YELLOW);
    printTitle(65, 3, "GAME RULES");
    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 4, "The Matching Game includes a board of multiple cells, each of which presents a figure. The player has to find");
    printTitle(20, 5, "and match a pair of cells that contain the same figure and connect each other in some particular pattern.");
    printTitle(20, 6, "The game ends when all matching pairs are found or run out of points or allowed mistakes or moves.");

    Screen::setConsoleColor(BLACK, YELLOW);
    printTitle(65, 9, "HOW TO PLAY");
    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 10, "There are four directions to move: ");
    Screen::setConsoleColor(BLACK, GREEN); std::cout<<"Up (Up Arrow), Down (Down arrow), Left (Left arrow), Right (Right arrow)";

    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 11, "Press ");
    printText(BLACK, GREEN, "Enter "); printText(BLACK, WHITE, "to select the cell, the second press will unselect the cell");

    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 12, "Press ");
    printText(BLACK, GREEN, "H "); printText(BLACK, WHITE, "to be suggested next move, which costs the player "); printText(BLACK, RED, "5 points");

    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 13, "Press ");
    printText(BLACK, GREEN, "F "); printText(BLACK, WHITE, "to shuffle the current board states, which costs the player "); printText(BLACK, RED, "9 points");

    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 14, "Press ");
    printText(BLACK, GREEN, "S "); printText(BLACK, WHITE, "to save the current board states, which just for ");
    printText(BLACK, PURPLE, "STANDARD MODE"); printText(BLACK, WHITE, " and "); printText(BLACK, PURPLE, "DIFFICULT MODE");

    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 15, "Press ");
    printText(BLACK, GREEN, "ESC "); printText(BLACK, WHITE, "to automatically save the player stats and back to main menu screen.");

    Screen::setConsoleColor(BLACK, YELLOW);
    printTitle(65, 18, "SCORING");
    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 19, "There are four matching patterns to allow the player to increase their current points: ");
    Screen::setConsoleColor(BLACK, PURPLE);
    printTitle(20, 20, "I Matching: "); printText(BLACK, GREEN, "3 points");
    Screen::setConsoleColor(BLACK, PURPLE);
    printTitle(20, 21, "L Matching: "); printText(BLACK, GREEN, "5 points");
    Screen::setConsoleColor(BLACK, PURPLE);
    printTitle(20, 22, "U Matching: "); printText(BLACK, GREEN, "7 points");
    Screen::setConsoleColor(BLACK, PURPLE);
    printTitle(20, 23, "Z Matching: "); printText(BLACK, GREEN, "7 points");
    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(20, 24, "When the player mismatched, it will decrease their allowed mistakes. The game ends if there is no allowed");
    printTitle(20, 25, "mistakes left. The player stats will be officially displayed on the leaderboard just for"); printText(BLACK, PURPLE, " CHALLENGE MODE"); printText(BLACK, WHITE, " only.");

    Screen::setConsoleColor(BLACK, YELLOW);
    printTitle(65, 28, "CREDITS");
    Screen::setConsoleColor(BLACK, WHITE);
    printTitle(55, 29, "Phan Thi Tuong Vi - 22127451");
    printTitle(55, 30, "Nguyen Trung Quan - 22127346");

    while (getch() != 27);
}
