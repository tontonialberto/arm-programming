#ifndef __ALBERTO_TONTONI_MOVEFUNCTIONS__
#define __ALBERTO_TONTONI_MOVEFUNCTIONS__

#include <stdlib.h>
#include "GameObject.h"
#include "GameConfig.h"

void Enemy_Move(void *_enemy);

void PlayerBullet_Move(GameObject *bullet);

void Enemy_Move(void *_enemy) {
	if(_enemy == NULL)
		return;
	
	Enemy *enemy = (Enemy *)_enemy;
	GameObject *go = &enemy->go;
	Rect2D *enemiesRect = &go->ctx->enemiesRect;
	Rect2D *rect = &go->rect;
	
	rect->x = enemiesRect->x + 
		(int16_t)((rect->width + ENEMY_HORIZ_SPACING) * (enemy->index % ENEMY_COLUMNS));
	rect->y = enemiesRect->y + 
		(int16_t)((rect->height + ENEMY_VERTICAL_SPACING) * (enemy->index / ENEMY_COLUMNS));
}

void PlayerBullet_Move(GameObject *bullet) {
	if(bullet == NULL)
		return;
	
	GameContext *ctx = bullet->ctx;
	
	if(bullet->active) {
		bullet->rect.y += ctx->playerBulletVerticalStep;
	}
	if(bullet->rect.y < ctx->gameArea.y) {
		bullet->active = false;
	}
	
	Enemy *overlappedEnemy = NULL;
	
	// Take the first enemy that overlaps with the bullet
	for(uint16_t i=0; i<ctx->nEnemies; i++) {
		Enemy *enemy = &ctx->enemies[i];
		
		if(Rect2D_Overlaps(enemy->go.rect, bullet->rect)) {
			overlappedEnemy = enemy;
			break;
		}
	}
	
	if(overlappedEnemy != NULL) {
		// TODO: Enemy_OnBulletCollision()
		bullet->active = false;
	}
}

#endif
