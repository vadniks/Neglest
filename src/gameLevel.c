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
#include "entity.h"
#include "../gameLib/include/gameLib.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <alloca.h>
#include <SDL2/SDL.h>

typedef struct {
    int x, y;
    Entity entity;
} Enemy;

struct GameLevel {
    Entity** field;
    int playerPositionX, playerPositionY;
    int collectedGems;
    int ticks;
    int totalGems;
    int enemiesSize;
    Enemy* enemies;
};

const int GAME_LEVEL_FIELD_ROWS = 50, GAME_LEVEL_FIELD_COLUMNS = 50;
static const int MAX_TICKS = 25;

static void addEnemy(GameLevel* level, int x, int y, Entity entity) {
    level->field[y][x] = entity;
    level->enemiesSize++;
    level->enemies = SDL_realloc(level->enemies, level->enemiesSize * sizeof(Enemy));
    level->enemies[level->enemiesSize - 1] = (Enemy) {x, y, entity};
}

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
    assert(getentropy(&entropy, sizeof(entropy)) == 0);
    srand(entropy);

    GameLevel* level = SDL_malloc(sizeof *level);
    level->collectedGems = 0;
    level->ticks = 0;
    level->totalGems = 0;
    level->enemiesSize = 0;
    level->enemies = nullptr;

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
                addEnemy(level, x, y, ENTITY_ENEMY_1);
                x++;
                break;
            case '2':
                addEnemy(level, x, y, ENTITY_ENEMY_2);
                x++;
                break;
            case '3':
                addEnemy(level, x, y, ENTITY_ENEMY_3);
                x++;
                break;
            case '4':
                addEnemy(level, x, y, ENTITY_ENEMY_4);
                x++;
                break;
            case '5':
                addEnemy(level, x, y, ENTITY_ENEMY_5);
                x++;
                break;
            case 'g':
                level->field[y][x] = ENTITY_GEM;
                level->totalGems++;
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
    SDL_free(level->enemies);
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
        level->collectedGems++;

    level->field[newPositionY][newPositionX] = ENTITY_PLAYER;
    level->field[level->playerPositionY][level->playerPositionX] = ENTITY_EMPTY;

    level->playerPositionX = newPositionX;
    level->playerPositionY = newPositionY;

    return true;
}

int gameLevelCollectedGems(const GameLevel* level) { return level->collectedGems; }

int gameLevelTotalGems(const GameLevel* level) { return level->totalGems; }

static void processEnemies(GameLevel* level) {
    for (int i = 0; i < level->enemiesSize; i++) {
        int x = level->enemies[i].x, y = level->enemies[i].y;

        switch (rand() / (RAND_MAX / 4)) {
            case 0:
                if (y > 0 && level->field[y - 1][x] == ENTITY_EMPTY) {
                    level->field[y - 1][x] = level->field[y][x];
                    level->field[y][x] = ENTITY_EMPTY;
                    level->enemies[i].y = y - 1;
                }
                break;
            case 1:
                if (x > 0 && level->field[y][x - 1] == ENTITY_EMPTY) {
                    level->field[y][x - 1] = level->field[y][x];
                    level->field[y][x] = ENTITY_EMPTY;
                    level->enemies[i].x = x - 1;
                }
                break;
            case 2:
                if (y < GAME_LEVEL_FIELD_ROWS - 1 && level->field[y + 1][x] == ENTITY_EMPTY) {
                    level->field[y + 1][x] = level->field[y][x];
                    level->field[y][x] = ENTITY_EMPTY;
                    level->enemies[i].y = y + 1;
                }
                break;
            case 3:
                if (x < GAME_LEVEL_FIELD_COLUMNS - 1 && level->field[y][x + 1] == ENTITY_EMPTY) {
                    level->field[y][x + 1] = level->field[y][x];
                    level->field[y][x] = ENTITY_EMPTY;
                    level->enemies[i].x = x + 1;
                }
                break;
        }
    }
}

static void processPlayer(GameLevel* level) {
    const int fieldSize = GAME_LEVEL_FIELD_ROWS * GAME_LEVEL_FIELD_COLUMNS * (int) sizeof(Entity*);
    const Entity** field = alloca(fieldSize);
    SDL_memcpy(field, level->field, fieldSize);

    int x, y;
    gameLibMove(gameCurrentLevel(), field, GAME_LEVEL_FIELD_ROWS, GAME_LEVEL_FIELD_COLUMNS, level->playerPositionX, level->playerPositionY, &x, &y);

    gameLevelTryMovePlayer(level, x, y);
}

void gameLevelUpdate(GameLevel* level) {
    level->ticks++;
    if (level->ticks < MAX_TICKS) return;
    level->ticks = 0;

    processPlayer(level);
    processEnemies(level);

    if (level->collectedGems == level->totalGems)
        gameChangeLevel();
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
