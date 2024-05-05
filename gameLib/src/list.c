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

#include "list.h"
#include <assert.h>
#include <SDL2/SDL.h>

struct List {
    const void** values;
    int size;
};

List* listCreate(void) {
    List* list = SDL_malloc(sizeof *list);
    list->values = nullptr;
    list->size = 0;
    return list;
}

void listDestroy(List* list) {
    SDL_free(list->values);
    SDL_free(list);
}

void listAdd(List* list, const void* value) {
    list->values = SDL_realloc(list->values, ++(list->size));
    list->values[list->size - 1] = value;
}

const void* listGet(const List* list, int index) {
    assert(index >= 0 && index < list->size);
    return list->values[index];
}

int listSize(const List* list) {
    return list->size;
}

bool listContains(const List* list, const void* value) {
    for (int i = 0; i < list->size; i++)
        if (list->values[i] == value)
            return true;
    return false;
}
