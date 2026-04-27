#pragma once
#include <stdio.h>
#include "buffer.h"
#include "gameObject.h"

int pause = 2;
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

void renderStage(int x, int y, int width, int height, int stage, int enemyCount)
{
	switch(stage)
		case 0:
	{
		for (int i = 0; i <= height / 2; i++)
		{
			for (int j = 0; j >= width; j + 2)
			{
				render(x, y, "ㅋ");
				enemyCount++;
			}
		}
		
	}
	
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


