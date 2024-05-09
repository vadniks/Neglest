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

#ifndef nullable
#   define nullable
#endif

struct Queue;
typedef struct Queue Queue;

typedef void (*QueueDeallocator)(void*);

Queue* queueCreate(QueueDeallocator deallocator);
void queueDestroy(Queue* queue);
void queuePush(Queue* queue, void* value);
bool queueEmpty(const Queue* queue);
void* nullable queuePull(Queue* queue);