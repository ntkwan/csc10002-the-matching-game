#include "screen.h"

HWND screen::console_window = GetConsoleWindow();
HANDLE screen::console_output = GetStdHandle(STD_OUTPUT_HANDLE);

void screen::setUpConsole(int fontSize) {
	setFontInfo(fontSize,fontSize);
	setAndCenterWindow();
	disableMaximize();
	setConsoleTitle();
	hideScrollBars();
	showCursor(false);
	disableMouseInput();
}

void screen::gotoXY(int pX, int pY) { SetConsoleCursorPosition(console_output, COORD{ (short)pX, (short)pY }); }

void screen::setAndCenterWindow() {
	RECT rect_client, rect_window;
	GetClientRect(console_window, &rect_client), GetWindowRect(console_window, &rect_window);

	const int width = 1152;
	const int height = 648;
	const int pos_x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
              pos_y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	MoveWindow(console_window, pos_x, pos_y, width, height, TRUE);
}

void screen::setConsoleColor(int background, int text) { SetConsoleTextAttribute(console_output, background * 16 + text); }

void screen::hideScrollBars() { ShowScrollBar(console_window, SB_BOTH, 0); }

void screen::setConsoleTitle() { SetConsoleTitle("THE MATCHING GAME"); }

void screen::disableMaximize() { SetWindowLong(console_window, GWL_STYLE, GetWindowLong(console_window, GWL_STYLE) & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME)); }

void screen::showCursor(bool show) {
	CONSOLE_CURSOR_INFO info = { 1, show };
	SetConsoleCursorInfo(console_output, &info);
}

void screen::setFontInfo(int size_x, int size_y) {
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(info);
	GetCurrentConsoleFontEx(console_output, FALSE, &info);
	info.dwFontSize.X = size_x;
	info.dwFontSize.Y = size_y;
	wcscpy_s(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(console_output, FALSE, &info);
}

void screen::clearConsole() { system("cls"); }

void screen::disableMouseInput() {
	DWORD prev_mode;
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

int screen::getConsoleInput() {
	int input_key = _getch();
    if (input_key == 0 || input_key == 224) {
        switch (_getch()) {
            case 72:
                return 2;
            case 75:
                return 3;
            case 77:
                return 4;
            case 80:
                return 5;
            default:
                return 0;
        }
    } else {
		if (input_key == 27)
            return 1;
		else if (input_key == 87 || input_key == 119)
			return 2;
		else if (input_key == 65 || input_key == 97)
			return 3;
		else if (input_key == 68 || input_key == 100)
			return 4;
		else if (input_key == 83 || input_key == 115)
			return 5;
		else if (input_key == 13)
			return 6;
		else if (input_key == 72 || input_key == 104)
			return 7;
		else if (input_key == 77 || input_key == 109)
			return 8;
		else
			return 0;
	}
}

void screen::playSound(int i) {}
