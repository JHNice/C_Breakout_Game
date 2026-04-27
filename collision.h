#pragma once
#include "gameObject.h"

int pause = 2;

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

void checkBrickCollision()
{

}