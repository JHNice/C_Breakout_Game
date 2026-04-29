#include <stdio.h>
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
	
	int playerSize = 11;
	int playerSpeed = 4;
	int x = width / 2 - playerSize;
	int y = height - 15;

	struct Player player = { x - playerSize, y, playerSpeed, playerSize };
	struct Ball ball = { x + playerSize, y - 2, 2.0f,1.0f,1 };
	struct Wall wall = { 0,0,0 };
	
	int tick = 0; // 공 속도 조절

	createMap(60, 40, width);

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
			renderBrick();
			renderScore(width, height);

			if (launch == 0)
			{
				ball.positionX = x + playerSize;
				ball.positionY = y - 1;

				renderBall((int)ball.positionX, (int)ball.positionY);
				render(width / 2 - 12, height - 5, "Z키를 입력해서 시작하세요");
			}
			else
			{
				if (tick >= 4)
				{
					checkWallCollision(&ball, width, height);
					checkPlayerCollision(&ball, x, y, player.size);
					ball.afterX = ball.positionX+ball.dX;
					ball.afterY = ball.positionY-ball.dY;
					checkBrickCollision(&ball, map, height);
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