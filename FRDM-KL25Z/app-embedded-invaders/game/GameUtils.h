#ifndef __ALBERTO_TONTONI_GAMEUTILS__
#define __ALBERTO_TONTONI_GAMEUTILS__

#include <stdlib.h>
#include "GameObject.h"

/// \brief Sets the intial position of a player bullet, given the player coordinates.
/// \params bullet The bullet which position has to be set.
/// \params [in] player The player object.
void SetSpawnPosition_PlayerBullet(GameObject *bullet, const GameObject *player);

/// \brief Check if a Rect2D was out of game area horizontal bounds,
/// 	and eventually restore its position.
/// \param obj The object that has to be checked and restored.
/// \param [in] bounds The bounds of the game area.
/// \return true if object was out of bounds, false otherwise.
bool RestoreInsideBoundsHoriz(Rect2D *obj, Rect2D *bounds);

/// \brief Find the shooter enemy for the next enemy bullet.
/// 	The shooter is the one with maximum y pos,
/// 	and the closest x pos with respect to the player.
/// \return NULL if no active enemies, otherwise a pointer to an active enemy.
Enemy *Enemy_GetNextShooter(const GameContext *ctx, const GameObject *player);

void SetSpawnPosition_PlayerBullet(GameObject *bullet, const GameObject *player) {
	if(player == NULL || bullet == NULL) 
		return;
	
	bullet->rect.x = player->rect.x + (bullet->rect.width / 2);
	bullet->rect.y = player->rect.y - (int16_t)bullet->rect.height;
}

bool RestoreInsideBoundsHoriz(Rect2D *obj, Rect2D *bounds) {
	bool wasOutOfBounds = false;
	if(obj == NULL || bounds == NULL)
		return wasOutOfBounds;
	
	if(obj->x < bounds->x) {
		obj->x = bounds->x;
		wasOutOfBounds = true;
	}
	else if(obj->x + obj->width > bounds->x + bounds->width) {
		obj->x = (int16_t)bounds->width - (int16_t)obj->width - (int16_t)1;
		wasOutOfBounds = true;
	}
	return wasOutOfBounds;
}

Enemy *Enemy_GetNextShooter(const GameContext *ctx, const GameObject *player) {
	
	Enemy *result = NULL;
	
	if(ctx == NULL || player == NULL) {
		return result;
	}
	
	Enemy * const enemies = ctx->enemies;
	const int16_t xPlayer = player->rect.x;
	
	// Find, among active enemies,
	// the x pos of the closest one to the player.
	uint16_t xMinDistanceFromPlayer = 65535;
	for(uint16_t i=0; i<ctx->nEnemies; i++) {
		
		if(enemies[i].go.active) {		
			const int16_t xEnemy = enemies[i].go.rect.x;
			const uint16_t xDistance = (uint16_t)abs(xEnemy - xPlayer);
			
			if(xDistance < xMinDistanceFromPlayer) {
				xMinDistanceFromPlayer = xDistance;
			}
		}
	}
	
	// Find, among active enemies, the one 
	// having maximum y pos.
	int16_t yMax = -1; // Initialized to the minimum value.
	for(uint16_t i=0; i<ctx->nEnemies; i++) {
		
		const uint16_t xDistance = (uint16_t)abs(enemies[i].go.rect.x - xPlayer);
		if(enemies[i].go.active && (xDistance == xMinDistanceFromPlayer)) {
			
			const int16_t yEnemy = enemies[i].go.rect.y;
			if(yMax < yEnemy) {
				yMax = yEnemy;
				result = &enemies[i];
			}
		}
	}
	
	return result;
}

#endif
