#include <stdio.h>
#include <time.h>
#include "render.h"

#define LEFT 75
#define RIGHT 77
#define SPACE 32


void setConsoleSizeSimple()
{
	system("mode con cols=100 lines=65");
}

int main()
{
	setConsoleSizeSimple();
	
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

	createMap(40, 20, width);

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

				hitScore = 0;
				itemScore = 0;
				player.life = 1;
				createMap(40, 20, width);
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
					timeStart = time(NULL);
				}
				break;
			case 90: // z
				if (launch == 0)
				{
					launch = !launch;
					timeStart = time(NULL);
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
				//TEMP
			case 105: // 공분열로 스테이지 클리어 테스트용 *i버튼
				if (pause == 0 && launch == 1) // 게임이 진행 중일 때만 작동하도록
				{
					int activeBall = 0;
					int currentActiveBall[MAX_BALLS];

					// 현재 활성화된 공들을 찾음
					for (int j = 0; j < MAX_BALLS; j++)
					{
						if (balls[j].active == 1)
						{
							currentActiveBall[activeBall] = j;
							activeBall++;
						}
					}

					// 활성화된 각 공마다 새로운 공을 하나씩 추가 (분열)
					for (int j = 0; j < activeBall; j++)
					{
						int originBall = currentActiveBall[j];

						for (int k = 0; k < MAX_BALLS; k++)
						{
							// 비활성화된 빈 공간을 찾아 새 공 생성
							if (balls[k].active == 0)
							{
								balls[k].positionX = balls[originBall].positionX;
								balls[k].positionY = balls[originBall].positionY;
								balls[k].dY = 1.0f;

								// 랜덤한 방향으로 튀도록 설정
								if (rand() % 10 < 5)
								{
									balls[k].dX = -balls[originBall].dX;
									if (rand() % 10 > 7)
									{
										balls[k].dY = -balls[originBall].dY;
									}
								}
								else
								{
									balls[k].dX = balls[originBall].dX;
								}

								balls[k].damage = 1;
								balls[k].active = 1;
								player.life++;

								break;
							}
						}
					}
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
			renderScore(width, height, &player);

			if (launch == 0)
			{
				balls[0].positionX = x + playerSize;
				balls[0].positionY = y - 1;

				renderBall((int)balls[0].positionX, (int)balls[0].positionY);
				render(width / 2 - 12, height - 5, "Z키를 입력해서 시작하세요");
			}
			else
			{
				if (tick >= 4)
				{

					for (int i = 0; i < MAX_BALLS; i++)
					{
						if (balls[i].active == 1)
						{
						checkWallCollision(&balls[i], width, height, &player);
						checkPlayerCollision(&balls[i], x, y, player.size);

						balls[i].afterX = balls[i].positionX+balls[i].dX;
						balls[i].afterY = balls[i].positionY-balls[i].dY;

						checkBrickCollision(&balls[i], map, width, height);

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