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
#include "queue.h"
#include <SDL2/SDL.h>

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
    if (oldX + 1 < fieldColumns && field[oldY][oldX + 1] == ENTITY_EMPTY || field[oldY][oldX + 1] == ENTITY_GEM) {
        *newX = oldX + 1;
        *newY = oldY;
    }

    int values[5] = {0, 1, 2, 3, 4};
    Queue* queue = queueCreate();

    queuePush(queue, &(values[0]));
    queuePush(queue, &(values[1]));
    queuePush(queue, &(values[2]));
    queuePush(queue, &(values[3]));
    queuePush(queue, &(values[4]));

    SDL_Log("%c", queueEmpty(queue) ? 't' : 'f');

    SDL_Log("%d", *(int*) queuePull(queue));
    SDL_Log("%d", *(int*) queuePull(queue));
    SDL_Log("%d", *(int*) queuePull(queue));
    SDL_Log("%d", *(int*) queuePull(queue));
    SDL_Log("%d", *(int*) queuePull(queue));

    SDL_Log("%c", queueEmpty(queue) ? 't' : 'f');

    queueDestroy(queue);
    SDL_Log("\n");
}

#pragma clang diagnostic pop
