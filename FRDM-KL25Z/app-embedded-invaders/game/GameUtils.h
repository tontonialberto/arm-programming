#ifndef __ALBERTO_TONTONI_GAMEUTILS__
#define __ALBERTO_TONTONI_GAMEUTILS__

#include <stdlib.h>
#include "GameObject.h"

/// \brief Sets the intial position of a player bullet, given the player coordinates.
/// \params [out] bullet The bullet which position has to be set.
/// \params [in] player The player object.
void SetSpawnPosition_PlayerBullet(GameObject *bullet, const GameObject *player);

void SetSpawnPosition_PlayerBullet(GameObject *bullet, const GameObject *player) {
	if(player == NULL || bullet == NULL) 
		return;
	
	bullet->x = player->x + (bullet->width / 2);
	bullet->y = player->y - (int16_t)bullet->height;
}

#endif
