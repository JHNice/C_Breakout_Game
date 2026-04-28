#pragma once

struct Player
{
	float positionX;
	float positionY;
	int speed;
	int size;
};

struct Ball
{
	float positionX;
	float positionY;
	float dX;
	float dY;
	int damage;
	float afterX;
	float afterY;
};

struct Wall
{
	int positionX;
	int positionY;
	int wallType;
};

typedef struct BRICK
{
	int positionX;
	int positionY;
	int hp;
}Brick;