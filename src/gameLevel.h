
#pragma once

#include "spriteRenderer.h"

typedef enum {
    GAME_LEVEL_ENTITY_EMPTY,
    GAME_LEVEL_ENTITY_BOX,
    GAME_LEVEL_ENTITY_PLAYER,
    GAME_LEVEL_ENTITY_ENEMY_1,
    GAME_LEVEL_ENTITY_ENEMY_2,
    GAME_LEVEL_ENTITY_ENEMY_3,
    GAME_LEVEL_ENTITY_ENEMY_4,
    GAME_LEVEL_ENTITY_ENEMY_5,
    GAME_LEVEL_ENTITY_GEM
} GameLevelEntity;

struct GameLevel;
typedef struct GameLevel GameLevel;

extern const int GAME_LEVEL_FIELD_ROWS, GAME_LEVEL_FIELD_COLUMNS;

GameLevel* gameLevelCreate(int which);
void gameLevelDestroy(GameLevel* level);
void gameLevelDraw(int cameraOffsetX, int cameraOffsetY, const GameLevel* level, const SpriteRenderer* renderer);
bool gameLevelCompleted(const GameLevel* level);
