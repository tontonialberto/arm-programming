#ifndef __ALBERTO_TONTONI_ON_COLLISION_FUNCTIONS__
#define __ALBERTO_TONTONI_ON_COLLISION_FUNCTIONS__

#include "GameObject.h"

void Enemy_OnBulletCollision(Enemy *enemy);

void Enemy_OnBulletCollision(Enemy *enemy) {
	if(enemy == NULL)
		return;
	
	enemy->go.active = false;
}

#endif
