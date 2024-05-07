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

#include "generation.h"
#include "game.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

typedef struct {
    int x, y;
} Point;

static int gRows = 0, gColumns = 0, gMaxRomWidth = 0, gMaxRoomHeight = 0;
char** gField = nullptr;

static int xRandom(int max) { return (int) ((float) random() / (float) RAND_MAX * ((float) max + 1.0f)); }

static void shuffleP(Point* array, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        Point t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

static bool listContainsPoint(const List* list, const Point* point) {
    for (int i = 0; i < listSize(list); i++) {
        const Point* nullable j = listGet(list, i);
        assert(j != nullptr);
        if (j->x == point->x && j->y == point->y)
            return true;
    }
    return false;
}

static void createPath(int x, int y, List* visited) {
    const int directionsSize = 4;
    Point directions[directionsSize] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    shuffleP(directions, directionsSize);

    for (int i = 0; i < directionsSize; i++) {
        const int
            dx = x + directions[i].x,
            dy = y + directions[i].y;

        if (dx >= 0 && dx < gColumns && dy >= 0 && dy < gRows) {
            char tile = gField[dy][dx];

            Point* point = SDL_malloc(sizeof *point);
            *point = (Point) {dx, dy};

            if (tile == 'b' && !listContainsPoint(visited, point)) {
                listAdd(visited, point);
                gField[dy][dx] = '.';
                createPath(dx, dy, visited);
            } else
                SDL_free(point);
        }
    }
}

void generationInit(void) {
    gRows = gameBlocksPerYAxis();
    gColumns = gameBlocksPerXAxis();
    gMaxRomWidth = gColumns / 2;
    gMaxRoomHeight = gRows / 2;

    gField = SDL_malloc(gRows * sizeof(char*));
    for (int i = 0; i < gRows; i++) {
        gField[i] = SDL_malloc(gColumns);
        for (int j = 0; j < gColumns; j++)
            gField[i][j] = 'b';
    }

    for (int y = 0; y < gRows; y++) {
        for (int x = 0; x < gColumns; x++) {
            printf("%c", gField[y][x]);
        }
        printf("\n");
    }
    printf("\n");

    int entropy;
    assert(getentropy(&entropy, sizeof(int)) == 0);
    srand(entropy);

    List* visited = listCreate(SDL_free);
    createPath(0, 0, visited);
    listDestroy(visited);

    for (int y = 0; y < gRows; y++) {
        for (int x = 0; x < gColumns; x++) {
            printf("%c", gField[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

void generationClean(void) {
    for (int i = 0; i < gRows; i++)
        SDL_free(gField[i]);
    SDL_free(gField);
}
