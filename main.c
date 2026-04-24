#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define LEFT 75
#define RIGHT 77

int index = 0;
HANDLE screen[2];
int size = sizeof(screen) / sizeof(screen[0]);

void initialize()
{
	CONSOLE_CURSOR_INFO cursor;

	// 화면 버퍼를 2개 생성합니다

	cursor.dwSize = 1; // 커서 두께 조절 1~100
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

void render(int x, int y, const char* character)
{
	DWORD dword;
	COORD position = { x, y };

	SetConsoleCursorPosition(screen[index], position);
	WriteFile(screen[index], character, strlen(character), &dword, NULL);
}


struct Player
{
	float positionX;
	float positionY;
	float speed;
	int size;

};

void renderPlayer(int x, int y, int size)
{
	for (int i = 0; i < size; i++)
	{
		render(x+(i*2), y, "ㅁ");
	}
}


int main()
{
	initialize();

	CONSOLE_SCREEN_BUFFER_INFO buffer;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &buffer);
	int width = (buffer.srWindow.Right) - buffer.srWindow.Left;
	int height = (buffer.srWindow.Bottom) - buffer.srWindow.Top;

	int x = width/2;
	int y = height - 5;

	char key = 0;

	int playerSize = 8;

	struct Player player = { (int)(width / 2) - playerSize, height - 5, 4, playerSize};

	while (1)
	{
		flip();

		clear();

		if (_kbhit())
		{
			key = _getch();
			if (key == -32 || key == 0)
			{
				key = _getch();
			}
			switch (key)
			{
			case LEFT:
				if (x - player.speed >= 0 )
				{
					x += -player.speed;
				}
				break;
			case RIGHT:
				if (x + player.speed + player.size * 2  < width)
				{
					x += player.speed;
				}
				break;
			default: render(x, y, "exception\n");
				break;
			}
		}

		renderPlayer(x, y, player.size);
	}

	release();

	return 0;
}