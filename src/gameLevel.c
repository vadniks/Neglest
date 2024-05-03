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

#include "gameLevel.h"
#include "game.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

typedef enum {
    ENTITY_EMPTY,
    ENTITY_BOX,
    ENTITY_PLAYER,
    ENTITY_ENEMY_1,
    ENTITY_ENEMY_2,
    ENTITY_ENEMY_3,
    ENTITY_ENEMY_4,
    ENTITY_ENEMY_5,
    ENTITY_GEM
} Entity;

struct GameLevel {
    Entity** field;
    int playerPositionX, playerPositionY;
    int gems;
    int ticks;
};

const int GAME_LEVEL_FIELD_ROWS = 50, GAME_LEVEL_FIELD_COLUMNS = 50;

GameLevel* gameLevelCreate(int which) {
    const int levelNameMaxSize = 64;
    char levelName[levelNameMaxSize];
    assert(SDL_snprintf(levelName, levelNameMaxSize, "res/level%d.txt", which) > 0);

    const int dataSize = GAME_LEVEL_FIELD_ROWS * GAME_LEVEL_FIELD_COLUMNS + GAME_LEVEL_FIELD_ROWS - 1;
    char data[dataSize];

    SDL_RWops* file = SDL_RWFromFile(levelName, "r");
    assert(file != nullptr);
    assert(SDL_RWread(file, data, 1, dataSize) == dataSize);
    SDL_RWclose(file);

    int entropy;
    getentropy(&entropy, sizeof(entropy));
    srand(entropy);

    GameLevel* level = SDL_malloc(sizeof *level);
    level->gems = 0;
    level->ticks = 0;

    level->field = SDL_malloc(GAME_LEVEL_FIELD_ROWS * sizeof(Entity*));
    for (int i = 0; i < GAME_LEVEL_FIELD_ROWS; i++) {
        level->field[i] = SDL_malloc(GAME_LEVEL_FIELD_COLUMNS * sizeof(Entity));
        for (int j = 0; j < GAME_LEVEL_FIELD_COLUMNS; j++)
            level->field[i][j] = ENTITY_EMPTY;
    }

    for (int xChar = 0, y = 0, x = 0; xChar < dataSize; xChar++) {
        switch (data[xChar]) {
            case 'b':
                level->field[y][x] = ENTITY_BOX;
                x++;
                break;
            case '.':
                level->field[y][x] = ENTITY_EMPTY;
                x++;
                break;
            case 'p':
                level->field[y][x] = ENTITY_PLAYER;
                level->playerPositionX = x;
                level->playerPositionY = y;
                x++;
                break;
            case '1':
                level->field[y][x] = ENTITY_ENEMY_1;
                x++;
                break;
            case '2':
                level->field[y][x] = ENTITY_ENEMY_2;
                x++;
                break;
            case '3':
                level->field[y][x] = ENTITY_ENEMY_3;
                x++;
                break;
            case '4':
                level->field[y][x] = ENTITY_ENEMY_4;
                x++;
                break;
            case '5':
                level->field[y][x] = ENTITY_ENEMY_5;
                x++;
                break;
            case 'g':
                level->field[y][x] = ENTITY_GEM;
                x++;
                break;
            case '\n':
                y++;
                x = 0;
                break;
            default:
                assert(false);
        }
    }

    return level;
}

void gameLevelDestroy(GameLevel* level) {
    for (int i = 0; i < GAME_LEVEL_FIELD_ROWS; i++)
        SDL_free(level->field[i]);
    SDL_free(level->field);
    SDL_free(level);
}

int gameLevelPlayerPositionX(const GameLevel* level) { return level->playerPositionX; }

int gameLevelPlayerPositionY(const GameLevel* level) { return level->playerPositionY; }

bool gameLevelTryMovePlayer(GameLevel* level, int newPositionX, int newPositionY) {
    if (newPositionX < 0 || newPositionX >= GAME_LEVEL_FIELD_COLUMNS) return false;
    if (newPositionY < 0 || newPositionY >= GAME_LEVEL_FIELD_ROWS) return false;

    const Entity entity = level->field[newPositionY][newPositionX];
    if (entity != ENTITY_EMPTY && entity != ENTITY_GEM) return false;

    if (entity == ENTITY_GEM)
        level->gems++;

    level->field[newPositionY][newPositionX] = ENTITY_PLAYER;
    level->field[level->playerPositionY][level->playerPositionX] = ENTITY_EMPTY;

    level->playerPositionX = newPositionX;
    level->playerPositionY = newPositionY;

    return true;
}

int gameLevelGems(const GameLevel* level) { return level->gems; }

void gameLevelUpdate(GameLevel* level) {
    level->ticks++;
    if (level->ticks < 100) return;
    level->ticks = 0;

    const int
        blocksPerYAxis = gameBlocksPerYAxis(),
        blocksPerXAxis = gameBlocksPerXAxis();

    for (int y = 0; y < blocksPerYAxis; y++) {
        for (int x = 0; x < blocksPerXAxis; x++) {
            bool enemy;

            switch (level->field[y][x]) {
                case ENTITY_ENEMY_1:
                    [[gnu::fallthrough]];
                case ENTITY_ENEMY_2:
                    [[gnu::fallthrough]];
                case ENTITY_ENEMY_3:
                    [[gnu::fallthrough]];
                case ENTITY_ENEMY_4:
                    [[gnu::fallthrough]];
                case ENTITY_ENEMY_5:
                    enemy = true;
                    break;
                default:
                    enemy = false;
                    break;
            }

            if (!enemy) continue;

            switch (rand() / (RAND_MAX / 4)) {
                case 0: // up
                    if (y > 0 && level->field[y - 1][x] == ENTITY_EMPTY) {
                        level->field[y - 1][x] = level->field[y][x];
                        level->field[y][x] = ENTITY_EMPTY;
                    }
                    break;
                case 1: // left
                    if (x > 0 && level->field[y][x - 1] == ENTITY_EMPTY) {
                        level->field[y][x - 1] = level->field[y][x];
                        level->field[y][x] = ENTITY_EMPTY;
                    }
                    break;
                case 2: // down
                    if (y < GAME_LEVEL_FIELD_ROWS - 1 && level->field[y + 1][x] == ENTITY_EMPTY) {
                        level->field[y + 1][x] = level->field[y][x];
                        level->field[y][x] = ENTITY_EMPTY;
                    }
                    break;
                case 3: // right
                    if (x < GAME_LEVEL_FIELD_COLUMNS - 1 && level->field[y][x + 1] == ENTITY_EMPTY) {
                        level->field[y][x + 1] = level->field[y][x];
                        level->field[y][x] = ENTITY_EMPTY;
                    }
                    break;
            }
        }
    }
}

void gameLevelDraw(const GameLevel* level, int cameraOffsetX, int cameraOffsetY, const SpriteRenderer* renderer) {
    const int
        blocksPerYAxis = gameBlocksPerYAxis(),
        blocksPerXAxis = gameBlocksPerXAxis();

    for (int y = 0; y < blocksPerYAxis; y++) {
        for (int x = 0; x < blocksPerXAxis; x++) {
            const Texture* nullable texture = nullptr;
            vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

            switch (level->field[y + cameraOffsetY][x + cameraOffsetX]) {
                case ENTITY_EMPTY:
                    texture = nullptr;
                    break;
                case ENTITY_BOX:
                    texture = gameTexture(GAME_TEXTURE_BOX);
                    SDL_memcpy(color, (vec4) {0.3f, 0.5f, 0.7f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_PLAYER:
                    texture = gameTexture(GAME_TEXTURE_PLAYER);
                    break;
                case ENTITY_ENEMY_1:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    SDL_memcpy(color, (vec4) {0.2f, 1.0f, 1.0f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_ENEMY_2:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    SDL_memcpy(color, (vec4) {0.4f, 0.8f, 0.8f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_ENEMY_3:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    SDL_memcpy(color, (vec4) {0.6f, 0.6f, 0.6f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_ENEMY_4:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    SDL_memcpy(color, (vec4) {0.8f, 0.4f, 0.4f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_ENEMY_5:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    SDL_memcpy(color, (vec4) {1.0f, 0.2f, 0.2f, 1.0f}, 4 * sizeof(float));
                    break;
                case ENTITY_GEM:
                    texture = gameTexture(GAME_TEXTURE_GEM);
                    break;
            }

            if (texture == nullptr) continue;

            spriteRendererDraw(
                renderer,
                texture,
                (vec2) {(float) (GAME_BLOCK_SIZE * x), (float) (GAME_BLOCK_SIZE * y)},
                (vec2) {(float) GAME_BLOCK_SIZE, (float) GAME_BLOCK_SIZE},
                0.0f,
                0.0f,
                0.0f,
                color
            );
        }
    }
}

bool gameLevelCompleted(const GameLevel* level) { return false; }
