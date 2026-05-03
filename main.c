#include <stdio.h>
#include "render.h"

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
	int itemTick = 0;
	

	struct Player player = { x - playerSize, y, playerSpeed, playerSize, 1 };
	struct Wall wall = { 0,0,0 };
	// struct Ball ball = { x + playerSize, y - 2, 2.0f,1.0f,1 };
	
	struct Ball balls[MAX_BALLS] = { 0 };
	balls[0].positionX = x + playerSize;
	balls[0].positionY = y - 2;
	balls[0].dX = 2.0f;
	balls[0].dY = 1.0f;
	balls[0].damage = 1;
	balls[0].active = 1;

	int tick = 0; // 공 속도 조절

	createMap(40, 30, width);

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

				for (int i = 0; i < MAX_BALLS; i++)
				{
					balls[i].active = 0;
				}

				balls[0].positionX = x + playerSize;
				balls[0].positionY = y - 2;
				balls[0].dX = 2.0f;
				balls[0].dY = -1.0f;
				balls[0].active = 1;
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
			renderFallingItems();
			renderScore(width, height);

			if (launch == 0)
			{
				balls[0].positionX = x + playerSize;
				balls[0].positionY = y - 1;

				renderBall((int)balls[0].positionX, (int)balls[0].positionY);
				render(width / 2 - 12, height - 5, "Z키를 입력해서 시작하세요");
			}
			else
			{
				if (tick >= 2)
				{
					for (int i = 0; i < MAX_BALLS; i++)
					{
						if (balls[i].active == 1)
						{
						checkWallCollision(&balls[i], width, height, &player);
						checkPlayerCollision(&balls[i], x, y, player.size);

						balls[i].afterX = balls[i].positionX+balls[i].dX;
						balls[i].afterY = balls[i].positionY-balls[i].dY;

						checkBrickCollision(&balls[i], map, height);

						launchGame(&balls[i]);
						}

					}


					player.positionX = x;
					player.positionY = y;

					checkItemCollision(&player, height, &balls);
					
					tick = 0;
				}
				
				for (int i = 0; i < MAX_BALLS; i++)
				{
					if (balls[i].active == 1)
					{
					renderBall((int)balls[i].positionX, (int)balls[i].positionY);
					}
				}
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