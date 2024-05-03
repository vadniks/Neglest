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

struct GameLevel;
typedef struct GameLevel GameLevel;

extern const int GAME_LEVEL_FIELD_ROWS, GAME_LEVEL_FIELD_COLUMNS;

GameLevel* gameLevelCreate(int which);
void gameLevelDestroy(GameLevel* nullable level);
int gameLevelPlayerPositionX(const GameLevel* level);
int gameLevelPlayerPositionY(const GameLevel* level);
bool gameLevelTryMovePlayer(GameLevel* level, int newPositionX, int newPositionY);
int gameLevelCollectedGems(const GameLevel* level);
int gameLevelTotalGems(const GameLevel* level);
void gameLevelUpdate(GameLevel* level);
void gameLevelDraw(const GameLevel* level, int cameraOffsetX, int cameraOffsetY, const SpriteRenderer* renderer);
