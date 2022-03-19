#ifndef __ALBERTO_TONTONI_GAMECONFIG__
#define __ALBERTO_TONTONI_GAMECONFIG__

#include <stdint.h>

#define N_ENEMIES (uint16_t)10U

extern const int16_t PLAYER_RIGHT_STEP;

extern const int16_t PLAYER_BULLET_UP_STEP;

extern const int16_t ENEMY_RIGHT_STEP;

extern const int16_t ENEMY_DOWN_STEP;

extern const int16_t ENEMY_BULLET_DOWN_STEP;

extern const uint16_t PLAYER_WIDTH;

extern const uint16_t PLAYER_HEIGHT;

extern const int16_t PLAYER_INITIAL_X;

extern const int16_t PLAYER_INITIAL_Y;

extern const uint16_t PLAYER_BULLET_WIDTH;

extern const uint16_t PLAYER_BULLET_HEIGHT;

extern const uint16_t ENEMY_WIDTH;

extern const uint16_t ENEMY_HEIGHT;

extern const uint16_t ENEMY_BULLET_WIDTH;

extern const uint16_t ENEMY_BULLET_HEIGHT;

extern const uint16_t ENEMY_COLUMNS;

extern const int16_t ENEMY_HORIZ_SPACING;

extern const int16_t ENEMY_VERTICAL_SPACING;

extern const int16_t ENEMIES_RECT_INITIAL_X;

extern const int16_t ENEMIES_RECT_INITIAL_Y;

extern const uint16_t ENEMIES_RECT_WIDTH;

extern const uint32_t EVT_ENEMY_MOVE_PERIOD_MS;

extern const uint32_t EVT_SPAWN_ENEMY_BULLET_PERIOD_MS;

extern const uint32_t SCORE_PER_ENEMY;

#endif
