#pragma once

#include <conio.h>
#include <windows.h>

int index = 0;
HANDLE screen[2];
int size = sizeof(screen) / sizeof(screen[0]);

void initialize()
{
	srand(time(NULL));

	CONSOLE_CURSOR_INFO cursor;

	cursor.dwSize = 1; // 커서 사이즈 조절 1~100
	cursor.bVisible = FALSE;

	for (int i = 0; i < size; i++)
	{
		screen[i] = CreateConsoleScreenBuffer
		(
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
		);

		SetConsoleCursorInfo(screen[i], &cursor);
	}
}

void flip()
{
	SetConsoleActiveScreenBuffer(screen[index]);
	index = !index;
}

void clear()
{
	COORD position = { 0,0 };

	DWORD dword;

	CONSOLE_SCREEN_BUFFER_INFO buffer;

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(console, &buffer);

	int width = (buffer.srWindow.Right + 1) - buffer.srWindow.Left;
	int height = (buffer.srWindow.Bottom + 1) - buffer.srWindow.Top;

	FillConsoleOutputCharacter(screen[index], ' ', width * height, position, &dword);
}

void release()
{
	for (int i = 0; i < size; i++)
	{
		CloseHandle(screen[i]);
	}
}

