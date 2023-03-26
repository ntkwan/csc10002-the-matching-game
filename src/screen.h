#ifndef SCREEN_H
#define SCREEN_H

#include <windows.h>
#include <conio.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define LIGHT_WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define WHITE 15

struct Screen
{
	static HWND console_window;
	static HANDLE console_output;

	Screen() {}
	~Screen() {}
	static void setUpConsole(int);
	static void gotoXY(int, int);
	static void setAndCenterWindow();
	static void setConsoleColor(int, int);
	static void hideScrollBars();
	static void setConsoleTitle();
	static void disableMaximize();
	static void showCursor(bool);
	static void setFontInfo(int, int);
	static void clearConsole();
	static void disableMouseInput();
	static int getConsoleInput();
	static void playSound(int);
};

#endif // SCREEN_H
