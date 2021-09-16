#pragma once

#include <stdio.h>
#include <stdarg.h>

#include <windows.h>

struct WinCons {
	WinCons() = delete;

	enum Color {
		Black = 0,
		Purple,
		Green,
		Blue,
		Red,
		Pink,
		Orange,
		White
	};

	static const int NUM_COLORS = static_cast<int>(Color::White) + 1;

	static const bool INTENSIFY = true;
	static const bool DO_NOT_INTENSIFY = false;

	static void color_printf(enum Color color, bool intensify, const char *format, ...)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		// Save the current colors used by the console in order to restore them
		// before returning to the caller.
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(console, &info);

		// Set the foreground color
		DWORD attributes = color;
		if (intensify)	attributes |= FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(console, static_cast<WORD>(attributes));

		// Printing text
		va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);

		// Restore the console colors
		SetConsoleTextAttribute(console, info.wAttributes);
	}
};

