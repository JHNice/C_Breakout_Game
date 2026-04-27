#include <stdio.h>
#include "buffer.h"
#include "gameObject.h"
#include "render.h"
#include "collision.h"

#define LEFT 75
#define RIGHT 77
#define SPACE 32


int main()
{
	initialize();

	CONSOLE_SCREEN_BUFFER_INFO buffer;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &buffer);
	int width = (buffer.srWindow.Right) - buffer.srWindow.Left;
	int height = (buffer.srWindow.Bottom) - buffer.srWindow.Top;

	char key = 0;
	
	int playerSize = 11; // 홀수
	int playerSpeed = 4;
	int x = width / 2 - playerSize;
	int y = height - 15;
	int stage = 0;
	int maxStage = 3;
	int enemyCount = 0;

	struct Player player = { x - playerSize, y, playerSpeed, playerSize };
	struct Ball ball = { x + playerSize, y - 2, 2.0f,1.0f,1 };
	struct Wall wall = { 0,0,0 };
	struct BRICK brick = { 0,0,1 };

	int tick = 0;

	while (1)
	{
		flip();

		clear();

		if (_kbhit())
		{
			if (pause == 3)
			{
				pause = 2;
				x = width / 2 - playerSize;
				y = height - 15;
				ball.positionX = x + playerSize;
				ball.positionY = y - 2;
				ball.dY = -ball.dY;
			}
			key = _getch();
			if (key == -32 || key == 0)
			{
				key = _getch();
			}
			switch (key)
			{
			case SPACE:
				pause = !pause;
				break;
				
			case 122: // Z
				if (launch == 0)
				{
					launch = !launch;
				}
				break;
			case 90: // z
				if (launch == 0)
				{
					launch = !launch;
				}
				break;

			case LEFT:
				if (x - player.speed > 0 )
				{
					x += -player.speed;
				}
				else
				{
					x = 0;
				}
				break;

			case RIGHT:
				if (x + player.speed + player.size * 2  < width)
				{
					x += player.speed;
				}
				else
				{
					x = width-player.size*2;
				}
				break;
			default: render(x, y, "exception\n");
				break;
			}
		}
		if (pause == 0)
		{
			renderMap(width, height);
			renderPlayer(x, y, player.size);
			renderStage(4, 2, width, height, stage, enemyCount);
			if (enemyCount <= 0 && stage < maxStage)
			{
				stage++;
				renderStage(4, 2, width, height, stage, enemyCount);
			}
			if (launch == 0)
			{
				ball.positionX = x + playerSize;
				ball.positionY = y - 1;

				renderBall((int)ball.positionX, (int)ball.positionY);
				render(width / 2 - 12, height - 5, "Z키를 입력해서 시작하세요");
			}
			else
			{
				if (tick >= 10)
				{
					checkWallCollision(&ball, width, height);
					checkPlayerCollision(&ball, x, y, player.size);
					checkBrickCollision();
					ball.lastX = ball.positionX;
					ball.lastY = ball.positionY;
					launchGame(&ball);
					tick = 0;
				}
				renderBall((int)ball.positionX, (int)ball.positionY);
			}
		}
		else
		{
			renderPause(width/2 - 5, height/2);
		}
		
		tick++;
	}

	release();

	return 0;
}