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

#include "queue.h"
#include <assert.h>
#include <SDL2/SDL.h>

struct Queue {
    const void** values;
    int size;
};

Queue* queueCreate(void) {
    Queue* queue = SDL_malloc(sizeof *queue);
    queue->values = nullptr;
    queue->size = 0;
    return queue;
}

void queueDestroy(Queue* queue) {
    SDL_free(queue->values);
    SDL_free(queue);
}

void queuePush(Queue* queue, const void* value) {
    queue->values = SDL_realloc(queue->values, ++(queue->size) * sizeof(void*));
    queue->values[queue->size - 1] = value;
}

bool queueEmpty(const Queue* queue) { return queue->size == 0; }

const void* queuePull(Queue* queue) {
    assert(queue->values);
    const void* value = queue->values[0];

    const int newSize = queue->size - 1;
    if (!newSize) {
        SDL_free(queue->values);
        queue->values = NULL;
        queue->size = 0;
        return value;
    }

    const void** temp = SDL_malloc(newSize * sizeof(void*));
    SDL_memcpy(temp, &(queue->values[1]), newSize * sizeof(void*));

    SDL_free(queue->values);
    queue->values = temp;
    queue->size = newSize;

    return value;
}
