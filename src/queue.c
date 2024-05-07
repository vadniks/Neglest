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
    void** values;
    int size;
    QueueDeallocator nullable deallocator;
};

Queue* queueCreate(QueueDeallocator nullable deallocator) {
    Queue* queue = SDL_malloc(sizeof *queue);
    queue->values = nullptr;
    queue->size = 0;
    queue->deallocator = deallocator;
    return queue;
}

void queueDestroy(Queue* queue) {
    if (queue->deallocator != nullptr)
        for (int i = 0; i < queue->size; queue->deallocator(queue->values[i++]));

    SDL_free(queue->values);
    SDL_free(queue);
}

void queuePush(Queue* queue, void* value) {
    queue->values = SDL_realloc(queue->values, ++(queue->size) * sizeof(void*));
    queue->values[queue->size - 1] = value;
}

bool queueEmpty(const Queue* queue) { return queue->size == 0; }

void* nullable queuePull(Queue* queue) {
    if (queue->size == 0 || queue->values == nullptr)
        return nullptr;

    void* value = queue->values[0];

    const int newSize = queue->size - 1;
    if (!newSize) {
        SDL_free(queue->values);
        queue->values = nullptr;
        queue->size = 0;
        return value;
    }

    void** temp = SDL_malloc(newSize * sizeof(void*));
    SDL_memcpy(temp, &(queue->values[1]), newSize * sizeof(void*));

    SDL_free(queue->values);
    queue->values = temp;
    queue->size = newSize;

    return value;
}
