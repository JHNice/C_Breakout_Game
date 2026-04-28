#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "gameObject.h"
#include "collision.h"



int launch = 0;
int score = 0;

void render(int x, int y, const char* character)
{
	DWORD dword;
	COORD position = { x, y };

	SetConsoleCursorPosition(screen[index], position);
	WriteFile(screen[index], character, strlen(character), &dword, NULL);
}

void renderPlayer(int x, int y, int size)
{
	for (int i = 0; i < size; i++)
	{
		render(x + (i * 2), y, "ㅁ");
	}
}

void renderBall(float x, float y)
{
	render(x, y, "ㅇ");
}


void renderPause(int x, int y)
{
	if (pause == 1)
	{
		render(x, y, "일 시 중 지!");
	}
	else if (pause == 2)
	{
		render(x - 19, y, "스페이스바를 입력하여 게임을 시작해주세요!");
	}
	else if (pause == 3)
	{
		render(x - 11, y, "게임에서 패배하셨습니다.");
	}
}

void renderMap(int width, int height)
{
	for (int i = 0; i < width / 2; i++)
	{
		render(i * 2, 0, "ㅡ");
	}
	for (int i = 0; i < height; i++)
	{
		render(0, i, "ㅣ");
		render(width-2, i, "ㅣ");
	}
}

Brick* createBrick(int x, int y)
{
	Brick* newBrick = (Brick*)malloc(sizeof(Brick));

	newBrick->positionX = x;
	newBrick->positionY = y;
	newBrick->hp = 1;

	return newBrick;
}


Brick* map[MAP_MAXSIZE_X][MAP_MAXSIZE_Y];

void createMap(int sizeX, int sizeY, int width)
{
	int totalSizeX = sizeX * 2;
	int startX = (width - totalSizeX) / 2;
	int startY = 4;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (x < MAP_MAXSIZE_X && y < MAP_MAXSIZE_Y)
			{
				map[x][y] = createBrick(startX + x*2,startY + y);
			}
		}
	}
}

void renderBrick()
{
	for (int y = 0; y < MAP_MAXSIZE_Y; y++)
	{
		for (int x = 0; x < MAP_MAXSIZE_X;x++)
		{
			if (map[x][y] != NULL && map[x][y]->hp > 0)
			{
				render(map[x][y]->positionX, map[x][y]->positionY, "ㅁ");
			}
		}
	}
}

void scoreCount()
{

}

void launchGame(struct Ball* ball)
{
	if (launch == 1)
	{
		ball->positionX += ball->dX;
		ball->positionY -= ball->dY;
	}
}


