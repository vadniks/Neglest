/*
 * Neglest - an OpenGL 2D game.
 * Copyright (C) 2024  Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
int gameLevelPlayerPositionX(const GameLevel* level);
int gameLevelPlayerPositionY(const GameLevel* level);
void gameLevelTryMovePlayer(GameLevel* level, int newPositionX, int newPositionY);
int gameLevelGems(const GameLevel* level);
void gameLevelDraw(const GameLevel* level, int cameraOffsetX, int cameraOffsetY, const SpriteRenderer* renderer);
bool gameLevelCompleted(const GameLevel* level);
