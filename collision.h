#pragma once
#include "gameObject.h"

int pause = 2;
int hitScore = 0;
int itemScore = 0;

void checkWallCollision(struct Ball* ball, int width, int height, struct Player* player)
{
	if (ball->positionX <= 2)
	{
		ball->dX = -ball->dX;
		if (rand() % 2 == 0)
		{
			ball->positionX += 2;
		}
	}
	else if (ball->positionX >= width - 2)
	{
		ball->dX = -ball->dX;
		if (rand() % 2 == 0)
		{
			ball->positionX -= 2;
		}
	}
	// Life 관리
	else if (ball->positionY >= height - 2)
	{
		player->life--;
		ball->active = 0;
		if (player->life <= 0)
		{
			pause = 3;
		}
	}
	else if (ball->positionY <= 2)
	{
		ball->dY = -ball->dY;
		if (rand() % 2 == 0)
		{
			ball->positionY += 1;
		}
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
				if (ball->dX >= 0 && ball->positionX <= playerX + playerSize -4)
				{
					if (rand() % 10 < 5)
					{
						ball->positionY--;
					}
					ball->dX = -ball->dX;
				}
				if (ball->dX < 0 && ball->positionX > playerX + playerSize +4)
				{
					if (rand() % 10 < 5)
					{
						ball->positionY--;
					}
					ball->dX = -ball->dX;
				}
			}
		}
	}

}

void checkBrickCollision(struct Ball* ball, Brick* map[MAP_MAXSIZE_X][MAP_MAXSIZE_Y], int height)
{
	int hitX = 0;
	int hitY = 0;

	for (int y = 0; y < MAP_MAXSIZE_Y; y++)
	{
		for (int x = 0;x < MAP_MAXSIZE_X;x++)
		{
			if (map[x][y] != NULL && map[x][y]->hp > 0)
			{
				int brickX = map[x][y]->positionX;
				int brickY = map[x][y]->positionY;


				if (hitY == 0 && (ball->positionX == brickX && ball->afterY == brickY))
				{
					ball->dY = -ball->dY;
					map[x][y]->hp = map[x][y]->hp - ball->damage;
					hitScore++;
					hitY = 1;

					if (map[x][y]->hp <= 0 && map[x][y]->item != -1)
					{
						for (int i = 0; i < MAX_FALLING_ITEMS; i++)
						{
							if (fallingItems[i].hp == 0)
							{
								fallingItems[i].positionX = map[x][y]->positionX;
								fallingItems[i].positionY = map[x][y]->positionY;
								fallingItems[i].itemNumber = map[x][y]->item;
								fallingItems[i].hp = 1;

								map[x][y]->item = -1;
								break;
							}
						}
					}
				}
				if (hitX == 0 && (ball->positionY == brickY && ball->afterX == brickX))
				{
					ball->dX = -ball->dX;
					map[x][y]->hp = map[x][y]->hp - ball->damage;
					hitScore++;
					hitX = 1;
					if (map[x][y]->hp <= 0 && map[x][y]->item != -1)
					{
						for (int i = 0; i < MAX_FALLING_ITEMS; i++)
						{
							if (fallingItems[i].hp == 0)
							{
								fallingItems[i].positionX = map[x][y]->positionX;
								fallingItems[i].positionY = map[x][y]->positionY;
								fallingItems[i].itemNumber = map[x][y]->item;
								fallingItems[i].hp = 1;

								map[x][y]->item = -1;
								break;
							}
						}
					}
				}
			}
		}
	}

	if (hitX == 0 && hitY == 0)
	{
		for (int y = 0; y < MAP_MAXSIZE_Y; y++)
		{
			for (int x = 0;x < MAP_MAXSIZE_X;x++)
			{
				if (map[x][y] != NULL && map[x][y]->hp > 0)
				{
					int brickX = map[x][y]->positionX;
					int brickY = map[x][y]->positionY;

					if (ball->afterX == brickX && ball->afterY == brickY)
					{
						ball->dX = -ball->dX;
						ball->dY = -ball->dY;
						map[x][y]->hp = map[x][y]->hp - ball->damage;
						hitScore++;

						if (map[x][y]->hp <= 0 && map[x][y]->item != -1)
						{
							for (int i = 0; i < MAX_FALLING_ITEMS; i++)
							{
								if (fallingItems[i].hp == 0)
								{
									fallingItems[i].positionX = map[x][y]->positionX;
									fallingItems[i].positionY = map[x][y]->positionY;
									fallingItems[i].itemNumber = map[x][y]->item;
									fallingItems[i].hp = 1;

									map[x][y]->item = -1;
									break;
								}
							}
						}

						return;
					}
				}
			}
		}
	}

}

void checkItemCollision(struct Player* player, int height, struct Ball* balls)
{
	for (int i = 0; i < MAX_FALLING_ITEMS; i++)
	{
		if (fallingItems[i].hp > 0)
		{
			fallingItems[i].positionY += 1;

			if (fallingItems[i].positionY >= height)
			{
				fallingItems[i].hp = 0;
				continue;
			}

			if (fallingItems[i].positionY == player->positionY)
			{
				if (fallingItems[i].positionX >= player->positionX && fallingItems[i].positionX <= player->positionX + (player->size * 2))
				{
					fallingItems[i].hp = 0;

					// 아이템 번호에 따라 효과 지정
					switch (fallingItems[i].itemNumber)
					{
					case 0:
						itemScore += 10; // 테스트, 아이템 점수 증가
						break;
					case 1:
						player->size += 2; // 사이즈 증가
						break;
					case 2: // 공 추가
						{
							int activeBall = 0;
							int currentActiveBall[MAX_BALLS];

							for (int j = 0;j < MAX_BALLS;j++)
							{
								if (balls[j].active == 1)
								{
									currentActiveBall[activeBall] = j;
									activeBall++;
								}
							}

							for (int j = 0;j < activeBall;j++)
							{
								int originBall = currentActiveBall[j];

								for (int k = 0; k < MAX_BALLS; k++)
								{
									if (balls[k].active == 0)
									{
										balls[k].positionX = balls[originBall].positionX;
										balls[k].positionY = balls[originBall].positionY;

										balls[k].dY = 1.0f;

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
										player->life++;

										break;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}
}