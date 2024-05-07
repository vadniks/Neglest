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

static int gRows = 0, gColumns = 0, gMaxRomWidth = 0, gMaxRoomHeight = 0;
char** gField = nullptr;

static int xRandom(int max) { return (int) ((float) random() / (float) RAND_MAX * ((float) max + 1.0f)); }

static void createPath(void) {
//    char current =
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

//    int entropy;
//    assert(getentropy(&entropy, sizeof(int)) == 0);
//    srand(entropy);

    //

//    for (int y = 0; y < gRows; y++) {
//        for (int x = 0; x < gColumns; x++) {
//            printf("%c", gField[y][x]);
//        }
//        printf("\n");
//    }
//    printf("\n");
}

void generationClean(void) {
    for (int i = 0; i < gRows; i++)
        SDL_free(gField[i]);
    SDL_free(gField);
}
