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

#include "../include/gameLib.h"
#include <assert.h>
#include <SDL2/SDL.h>

static int gCurrentLevel = 0;
static const Entity* const* gField = nullptr;
static int gFieldRows = 0, gFieldColumns = 0;

void gameLibInit(int currentLevel, const Entity* const* field, int fieldRows, int fieldColumns) {
    gCurrentLevel = currentLevel;
    gField = field;
    gFieldRows = fieldRows;
    gFieldColumns = fieldColumns;
}

static GameLibDirection move(int x, int y) {
    return GAME_LIB_DIRECTION_RIGHT;
}

GameLibDirection gameLibMove(int x, int y) {
    const int allocations = SDL_GetNumAllocations();
    const GameLibDirection direction = move(x, y);
    assert(allocations == SDL_GetNumAllocations());
    return direction;
}
