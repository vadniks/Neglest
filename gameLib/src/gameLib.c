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

typedef struct {
    int x, y;
} Gem;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
#pragma clang diagnostic ignored "-Wunused-parameter"

void gameLibMove(
    int currentLevel,
    const Entity** field,
    int fieldRows,
    int fieldColumns,
    int oldX,
    int oldY,
    int* newX,
    int* newY
) {
    const int allocations = SDL_GetNumAllocations();

    Gem* gems = nullptr;
    int gemsSize = 0;

    for (int y = 0; y < fieldRows; y++) {
        for (int x = 0; x < fieldColumns; x++) {
            if (field[y][x] == ENTITY_GEM) {
                gems = SDL_realloc(gems, ++gemsSize * sizeof(Entity));
                gems[gemsSize - 1] = (Gem) {x, y};
            }
        }
    }

    for (int i = 0; i < gemsSize; i++)
        SDL_Log("x: %d, y: %d", gems[i].x, gems[i].y);
    SDL_Log("");

    SDL_free(gems);

    assert(allocations == SDL_GetNumAllocations());
}

#pragma clang diagnostic pop
