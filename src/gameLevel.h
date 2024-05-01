
#pragma once

#include "spriteRenderer.h"

typedef enum {
    GAME_LEVEL_ENTITY_EMPTY,
    GAME_LEVEL_ENTITY_BOX,
    GAME_LEVEL_ENTITY_PLAYER,
    GAME_LEVEL_ENTITY_ENEMY,
    GAME_LEVEL_ENTITY_GEM
} GameLevelEntity;

struct GameLevel;
typedef struct GameLevel GameLevel;

GameLevel* gameLevelCreate(int which);
void gameLevelDestroy(GameLevel* level);
void gameLevelDraw(int cameraOffsetX, int cameraOffsetY, const GameLevel* level, const SpriteRenderer* renderer);
bool gameLevelCompleted(const GameLevel* level);
