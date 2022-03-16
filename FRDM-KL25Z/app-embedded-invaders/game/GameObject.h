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
	Enemy *enemies;
	uint16_t nEnemies;
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

void GameContext_Init(
	GameContext *ctx,
	int8_t enemyHorizontalDirection,
	int16_t playerBulletVerticalStep,
	const Rect2D *enemiesRect,
	const Rect2D *gameArea);
	
bool Rect2D_Overlaps(const Rect2D a, const Rect2D b);

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
		
void GameContext_Init(
		GameContext *ctx,
		int8_t enemyHorizontalDirection,
		int16_t playerBulletVerticalStep,
		const Rect2D *enemiesRect,
		const Rect2D *gameArea) {
	
	if(ctx == NULL || enemiesRect == NULL || gameArea == NULL)
		return;
			
	ctx->enemiesRect = *enemiesRect;
	ctx->enemyHorizontalDirection = enemyHorizontalDirection;
	ctx->enemyHorizontalStep = 0;
	ctx->enemyVerticalStep = 0;
	ctx->gameArea = *gameArea;
	ctx->hasEnemyHitBoundary = false;
	ctx->playerBulletVerticalStep = playerBulletVerticalStep;
	ctx->playerHorizontalStep = 0;
	ctx->spawnPlayerBullet = false;
}
		
bool Rect2D_Overlaps(const Rect2D a, const Rect2D b) {
	// (x2, y2) is the bottom-right edge of the rectangle.
	const int16_t a_x2 = a.x + (int16_t)a.width;
	const int16_t a_y2 = a.y + (int16_t)a.height;
	const int16_t b_x2 = b.x + (int16_t)b.width;
	const int16_t b_y2 = b.y + (int16_t)b.height;
	
	return (a.x < b_x2) && (a_x2 > b.x) && (a_y2 > b.y) && (a.y < b_y2);
}

#endif
