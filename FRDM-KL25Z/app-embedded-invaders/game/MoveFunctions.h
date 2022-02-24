#ifndef __ALBERTO_TONTONI_MOVEFUNCTIONS__
#define __ALBERTO_TONTONI_MOVEFUNCTIONS__

#include <stdlib.h>
#include "GameObject.h"
#include "GameConfig.h"

void Enemy_Move(void *_enemy);

void Enemy_Move(void *_enemy) {
	if(_enemy == NULL)
		return;
	
	Enemy *enemy = (Enemy *)_enemy;
	GameObject *go = &enemy->go;
	Rect2D *enemiesRect = &go->ctx->enemiesRect;
	Rect2D *rect = &go->rect;
	
	rect->x = enemiesRect->x + (int16_t)(rect->width * (enemy->index % ENEMY_COLUMNS));
	rect->y = enemiesRect->y + (int16_t)(rect->height * (enemy->index / ENEMY_COLUMNS));
}

#endif
