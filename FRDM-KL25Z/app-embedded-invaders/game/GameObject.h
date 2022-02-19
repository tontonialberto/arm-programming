#ifndef __ALBERTO_TONTONI_GAMEOBJECT__
#define __ALBERTO_TONTONI_GAMEOBJECT__

#include <stdint.h>
#include <stdbool.h>

struct GameObject;
typedef struct GameObject GameObject;

struct GameContext;
typedef struct GameContext GameContext;

typedef void (* MoveFn)(GameObject *, GameContext *);
typedef void (* RenderFn)(GameObject *);

struct GameContext {
	int16_t playerHorizontalStep;
	int16_t playerBulletVerticalStep;
	int16_t enemyHorizontalStep;
	int8_t enemyHorizontalDirection;
	int16_t enemyVerticalStep;
	int16_t gameAreaMinX;
	int16_t gameAreaMaxX;
	int16_t gameAreaMinY;
	bool spawnPlayerBullet;
};

struct GameObject {
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	GameContext *ctx;
	bool active;
	int16_t id;
	
	MoveFn move;
	RenderFn render;
};

#endif
