#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "gameObject.h"
#include "collision.h"

int launch = 0;
int aliveBrick = 0;
time_t timeStart = 0.00f;

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
	else if (pause == 100)
	{
		int clearScore = hitScore;
		int clearTime = timeStart;
		char clearScoreText[10];
		char clearTimeText[20];
		_itoa_s(hitScore, clearScoreText, sizeof(clearScoreText), 10);
		_itoa_s(clearTime, clearTimeText, sizeof(clearTimeText), 20);
		render(x, y, "stage clear");
		render(x, y + 1, "최종 점수 : ");
		render(x+14, y + 1, clearScoreText);
		render(x, y + 2, "클리어 타임 : ");
		render(x+16, y + 2, clearTimeText);
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

	if (rand() % 100 < 10)
	{
		newBrick->item = rand() % 3;
	}
	else
	{
		newBrick->item = -1;
	}

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
				if (map[x][y]->positionX == startX || map[x][y]->positionX == startX+(sizeX*2)-2)
				{
					map[x][y]->hp = 10;
				}
				else if (map[x][y]->positionY == startY + sizeY-1)
				{
					map[x][y]->hp = 10;
				}
			}
		}
	}
}

void stageClear()
{
	for (int x = 0; x < MAP_MAXSIZE_X; x++)
	{
		for (int y = 0; y < MAP_MAXSIZE_Y;y++)
		{
			if (map[x][y] != NULL && map[x][y]->hp >= 1)
			{
				aliveBrick++;
			}
		}
	}
	if (aliveBrick <= 0)
	{
		pause = 100;
	}
	aliveBrick = 0;
}

void renderBrick()
{
	for (int y = 0; y < MAP_MAXSIZE_Y; y++)
	{
		for (int x = 0; x < MAP_MAXSIZE_X;x++)
		{
			if (map[x][y] != NULL && map[x][y]->hp > 0)
			{
				if (map[x][y]->hp > 5)
				{
					render(map[x][y]->positionX, map[x][y]->positionY, "■");
				}
				else if (map[x][y]->hp > 1)
				{
					render(map[x][y]->positionX, map[x][y]->positionY, "□");
				}
				else
				{
					render(map[x][y]->positionX, map[x][y]->positionY, "ㅁ");
				}
				
			}
		}
	}
}

void renderFallingItems()
{
	for (int i = 0; i < MAX_FALLING_ITEMS; i++)
	{
		if (fallingItems[i].hp > 0)
		{
			switch (fallingItems[i].itemNumber)
			{
			case 0:
				render(fallingItems[i].positionX, fallingItems[i].positionY, "♥");
				break;
			case 1:
				render(fallingItems[i].positionX, fallingItems[i].positionY, "★");
				break;
			case 2:
				render(fallingItems[i].positionX, fallingItems[i].positionY, "＠");
				break;
			}
		}
	}
}

void renderScore(int width, int height, struct Player* player)
{
	char scoreText[20];
	char itemScoreText[20];
	char playerLifeText[10];
	char stageTimeText[20];

	int elapsedTime = 0;
	if (launch == 1) {
		elapsedTime = (int)(time(NULL) - timeStart);
	}
	_itoa_s(elapsedTime, stageTimeText, sizeof(stageTimeText), 10);

	_itoa_s(hitScore, scoreText, sizeof(scoreText), 10);
	_itoa_s(itemScore, itemScoreText, sizeof(itemScoreText), 10);
	_itoa_s(player->life, playerLifeText, sizeof(playerLifeText), 10);
	_itoa_s(timeStart, stageTimeText, sizeof(stageTimeText), 20);
	render(4, height - 5, "점수 : ");
	render(12, height - 5, scoreText);
	render(4, height - 6, "itemScore : ");
	render(14, height - 6, itemScoreText);
	render(4, height - 4, "LIFE : ");
	render(12, height - 4, playerLifeText);
	render(4, height - 3, "TIME : ");
	render(12, height - 3, stageTimeText);
}

void launchGame(struct Ball* ball)
{
	if (launch == 1)
	{
		ball->positionX += ball->dX;
		ball->positionY -= ball->dY;
	}
}


