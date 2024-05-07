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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

static int xRandom(int max) { return (int) random() / RAND_MAX * (max + 1); }

static int min(int a, int b) { return a < b ? a : b; }

void generation(void) {
    const int
        rows = gameBlocksPerYAxis(),
        columns = gameBlocksPerXAxis(),
        maxWidth = columns / 4,
        maxHeight = rows / 4;

    char field[rows][columns];
    SDL_memset(field, '.', sizeof field);

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            printf("%c", field[y][x]);
        }
        printf("\n");
    }
    printf("\n");

    int entropy;
    assert(getentropy(&entropy, sizeof(int)) == 0);
    srand(entropy);

    const int roomsCount = 5;
    for (int i = 0; i < roomsCount; i++) {
        const int
            width = xRandom(maxWidth),
            height = xRandom(maxHeight),
            initialX = abs(xRandom(columns) - width),
            initialY = abs(xRandom(rows) - height);

        for (int y = initialY; y < height; y++) {
            for (int x = initialX; x < width; x++) {
                if (y < rows && x < columns)
                    field[y][x] = 'b';
            }
        }
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            printf("%c", field[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}
