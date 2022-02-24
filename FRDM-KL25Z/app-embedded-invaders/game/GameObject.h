#ifndef __ALBERTO_TONTONI_GAMEOBJECT__
#define __ALBERTO_TONTONI_GAMEOBJECT__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct Rect2D;
typedef struct Rect2D Rect2D;

struct GameObject;
typedef struct GameObject GameObject;

struct GameContext;
typedef struct GameContext GameContext;

struct Enemy;
typedef struct Enemy Enemy;

typedef void (* MoveFn)(GameObject *, GameContext *);
typedef void (* RenderFn)(GameObject *);

struct Rect2D {
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
};

struct GameContext {
	int16_t playerHorizontalStep;
	int16_t playerBulletVerticalStep;
	int16_t enemyHorizontalStep;
	int16_t enemyVerticalStep;
	int8_t enemyHorizontalDirection;
	bool hasEnemyHitBoundary;
	Rect2D enemiesRect;
	Rect2D gameArea;
	bool spawnPlayerBullet;
};

struct GameObject {
	MoveFn move;
	RenderFn render;
	
	Rect2D rect;
	GameContext *ctx;
	bool active;
};

struct Enemy {
	GameObject go;
	int16_t index;
};

void Rect2D_Init(
	Rect2D *rect, 
	int16_t x, 
	int16_t y,
	uint16_t width,
	uint16_t height);

void Rect2D_Init(
		Rect2D *rect, 
		int16_t x, 
		int16_t y,
		uint16_t width,
		uint16_t height) {
	
	if(rect == NULL) 
		return;
	
	rect->x = x;
	rect->y = y;
	rect->width = width;
	rect->height = height;
}

#endif
