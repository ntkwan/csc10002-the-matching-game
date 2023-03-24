#ifndef SCREEN_H
#define SCREEN_H

#include <windows.h>
#include <conio.h>

struct Screen
{
	static HWND console_window;
	static HANDLE console_output;

	Screen() {}
	~Screen() {}
	static void setUpConsole(int fontSize);
	static void gotoXY(int, int);
	static void setAndCenterWindow();
	static void setConsoleColor(int, int);
	static void hideScrollBars();
	static void setConsoleTitle();
	static void disableMaximize();
	static void showCursor(bool);
	static void setFontInfo(int sizeX, int sizeY);
	static void clearConsole();
	static void disableMouseInput();
	static int getConsoleInput();
	static void playSound(int);
};

#endif // SCREEN_H
