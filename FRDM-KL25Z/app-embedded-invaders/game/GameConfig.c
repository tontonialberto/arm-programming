#include "GameConfig.h"

const int16_t PLAYER_RIGHT_STEP = 3;

const int16_t PLAYER_BULLET_UP_STEP = -3;

const int16_t ENEMY_RIGHT_STEP = 10;

const int16_t ENEMY_DOWN_STEP = 5;

const int16_t ENEMY_BULLET_DOWN_STEP = +2;

const uint16_t PLAYER_WIDTH = 6;

const uint16_t PLAYER_HEIGHT = 3;

const int16_t PLAYER_INITIAL_X = 56;

const int16_t PLAYER_INITIAL_Y = 28;

const uint16_t PLAYER_BULLET_WIDTH = 1;

const uint16_t PLAYER_BULLET_HEIGHT = 2;

const uint16_t ENEMY_WIDTH = 8;

const uint16_t ENEMY_HEIGHT = 2;

const uint16_t ENEMY_BULLET_WIDTH = 1;

const uint16_t ENEMY_BULLET_HEIGHT = 1;

const uint16_t ENEMY_COLUMNS = 5;

const int16_t ENEMY_HORIZ_SPACING = 6 ;

const int16_t ENEMY_VERTICAL_SPACING = 5;

const int16_t ENEMIES_RECT_INITIAL_X = 30;

const int16_t ENEMIES_RECT_INITIAL_Y = 5;

const uint16_t ENEMIES_RECT_WIDTH = 
		(ENEMY_WIDTH * ENEMY_COLUMNS) + (uint16_t)(ENEMY_HORIZ_SPACING * (ENEMY_COLUMNS - 1));

const uint32_t EVT_ENEMY_MOVE_PERIOD_MS = 1000;

const uint32_t EVT_SPAWN_ENEMY_BULLET_PERIOD_MS = 2000;

const uint32_t SCORE_PER_ENEMY = 50;
