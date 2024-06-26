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

struct List;
typedef struct List List;

typedef void (*ListDeallocator)(void*);

List* listCreate(ListDeallocator deallocator);
void listDestroy(List* list);
void listAdd(List* list, void* value);
void* nullable listGet(const List* list, int index);
int listSize(const List* list);
