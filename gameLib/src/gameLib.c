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
#include "queue.h"
#include <gameLib/gameLib.h>
#include <assert.h>
#include <SDL2/SDL.h>

static int gCurrentLevel = 0;
static const Entity* const* gField = nullptr;
static int gFieldRows = 0, gFieldColumns = 0;

void gameLibInit(int currentLevel, int fieldRows, int fieldColumns) {
    gCurrentLevel = currentLevel;
    gFieldRows = fieldRows;
    gFieldColumns = fieldColumns;
}

static Direction move(int x, int y);

Direction gameLibMove(const Entity* const* field, int x, int y) {
    gField = field;

    const int allocations = SDL_GetNumAllocations();
    const Direction direction = move(x, y);
    assert(allocations == SDL_GetNumAllocations());

    return direction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// https://github.com/Gribbirg/DandyBotSolution/blob/main/user_bot.py ⮧
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int x, y;
} Coordinates;

typedef struct {
    Direction direction;
    Coordinates coordinates;
} Node;

static Queue* baseQueue(int x, int y) {
    Queue* queue = queueCreate(SDL_free);

    const int size = 4;
    Direction directions[size] = {DIRECTION_LEFT, DIRECTION_RIGHT, DIRECTION_UP, DIRECTION_DOWN};
    Coordinates neighbours[size] = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};

    for (int i = 0; i < size; i++) {
        Node* node = SDL_malloc(sizeof *node);
        node->direction = directions[i];
        node->coordinates = neighbours[i];
        queuePush(queue, node);
    }

    return queue;
}

static void addToQueue(Queue* queue, int x, int y, Direction direction) {
    const int size = 4;
    Coordinates neighbours[size] = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};

    for (int i = 0; i < size; i++) {
        Node* node = SDL_malloc(sizeof *node);
        node->direction = direction;
        node->coordinates = neighbours[i];
        queuePush(queue, node);
    }
}

static bool check(Entity entity, int x, int y) { return gField[y][x] == entity; }

static Direction move(int x, int y) {
    List* visited = listCreate(SDL_free);
    Queue* queue = baseQueue(x, y);

    Direction directionToReturn;

    while (true) {
        Node* node = queuePull(queue);
        Direction direction = node->direction;
        Coordinates coordinates = node->coordinates;
        SDL_free(node);

        if (check(ENTITY_GEM, coordinates.x, coordinates.y)) {
            directionToReturn = direction;
            goto end;
        }

        bool xContinue = false;
        for (int i = 0; i < listSize(visited); i++) {
            Coordinates* xCoordinates = listGet(visited, i);
            if (coordinates.x == xCoordinates->x && coordinates.y == xCoordinates->y)
                xContinue = true;
        }
        if (check(ENTITY_BOX, coordinates.x, coordinates.y))
            xContinue = true;

        if (xContinue)
            continue;

        addToQueue(queue, coordinates.x, coordinates.y, direction);

        Coordinates* xCoordinates = SDL_malloc(sizeof *xCoordinates);
        *xCoordinates = coordinates;
        listAdd(visited, xCoordinates);
    }
    end:

    listDestroy(visited);
    queueDestroy(queue);

    SDL_Log("%d", directionToReturn);
    return directionToReturn;
}
