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

#endif
