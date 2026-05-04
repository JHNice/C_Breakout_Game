#pragma once

#define MAX_FALLING_ITEMS 20
#define MAP_MAXSIZE_X 90
#define MAP_MAXSIZE_Y 30
#define MAX_BALLS 100

struct Player
{
	float positionX;
	float positionY;
	int speed;
	int size;
	int life;
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
	int active;
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
	int item;
}Brick;

typedef struct ITEM
{
	int positionX;
	int positionY;
	int hp;
	int itemNumber;
}Item;

Item fallingItems[MAX_FALLING_ITEMS];