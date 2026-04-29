#pragma once
#include "gameObject.h"

#define MAP_MAXSIZE_X 90
#define MAP_MAXSIZE_Y 30

int pause = 2;
int hitScore = 0;

void checkWallCollision(struct Ball* ball, int width, int height)
{
	if (ball->positionX <= 1)
	{
		ball->dX = -ball->dX;
	}
	else if (ball->positionX >= width - 2)
	{
		ball->dX = -ball->dX;
	}
	else if (ball->positionY >= height - 2)
	{
		pause = 3;
	}
	else if (ball->positionY <= 1)
	{
		ball->dY = -ball->dY;
	}
}

void checkPlayerCollision(struct Ball* ball, int playerX, int playerY, int playerSize)
{
	if (ball->positionY == playerY - 1)
	{
		if (ball->positionX >= playerX && ball->positionX <= playerX + (playerSize * 2))
		{
			if (ball->dY < 0)
			{
				ball->dY = -ball->dY;
			}
		}
	}

}

void checkBrickCollision(struct Ball* ball, Brick* map[MAP_MAXSIZE_X][MAP_MAXSIZE_Y], int height)
{
	for (int y = 0; y < MAP_MAXSIZE_Y; y++)
	{
		for (int x = 0;x < MAP_MAXSIZE_X;x++)
		{
			if (map[x][y] != NULL && map[x][y]->hp > 0)
			{
				int brickX = map[x][y]->positionX;
				int brickY = map[x][y]->positionY;
				
				if (ball->positionX == brickX && ball->afterY == brickY)
				{
					ball->dY = -ball->dY;
					map[x][y]->hp = map[x][y]->hp - ball->damage;
					hitScore++;
					itemDrop(map[x][y]->positionX,map[x][y]->positionY, height);
					return;
				}
				if (ball->positionY == brickY && ball->afterX == brickX)
				{
					ball->dX = -ball->dX;
					map[x][y]->hp = map[x][y]->hp - ball->damage;
					hitScore++;
					itemDrop(map[x][y]->positionX, map[x][y]->positionY, height);
					return;
				}
			}
		}
	}
}