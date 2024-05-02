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

#include "game.h"
#include "compoundShader.h"
#include "spriteRenderer.h"
#include "gameLevel.h"
#include <cglm/cam.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int GAME_BLOCK_SIZE = 50, GAME_WINDOW_WIDTH = GAME_BLOCK_SIZE * 32, GAME_WINDOW_HEIGHT = GAME_BLOCK_SIZE * 18;
//         columns              rows
static int gBlocksPerXAxis = 0, gBlocksPerYAxis = 0;
static CompoundShader* gSpriteShader = nullptr;
static SpriteRenderer* gSpriteRenderer = nullptr;
static Texture* gBoxTexture = nullptr;
static Texture* gPlayerTexture = nullptr;
static Texture* gEnemyTexture = nullptr;
static Texture* gGemTexture = nullptr;
static GameLevel* gGameLevel = nullptr;
static int gCameraOffsetX = 0, gCameraOffsetY = 0;

static Texture* loadTextureAndConvertFormat(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    assert(surface != nullptr);

    SDL_Surface* xSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);

    Texture* texture = textureCreate(xSurface->w, xSurface->h, xSurface->pixels);
    SDL_FreeSurface(xSurface);
    return texture;
}

void gameInit(void) {
    gBlocksPerXAxis = GAME_WINDOW_WIDTH / GAME_BLOCK_SIZE;
    gBlocksPerYAxis = GAME_WINDOW_HEIGHT / GAME_BLOCK_SIZE;

    mat4 projection;
    glm_ortho(0.0f, (float) GAME_WINDOW_WIDTH, (float) GAME_WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);

    gSpriteShader = compoundShaderCreate("shaders/spriteVertex.glsl", "shaders/spriteFragment.glsl");
    compoundShaderUse(gSpriteShader);
    compoundShaderSetMat4(gSpriteShader, "projection", projection);
    compoundShaderSetInt(gSpriteShader, "sprite", 0);

    gSpriteRenderer = spriteRendererCreate(gSpriteShader);
    gBoxTexture = loadTextureAndConvertFormat("res/box.png");
    gPlayerTexture = loadTextureAndConvertFormat("res/player_a.png");
    gEnemyTexture = loadTextureAndConvertFormat("res/enemy_a.png");
    gGemTexture = loadTextureAndConvertFormat("res/gem.png");

    gGameLevel = gameLevelCreate(0);
}

int gameBlocksPerXAxis(void) {
    return gBlocksPerXAxis;
}

int gameBlocksPerYAxis(void) {
    return gBlocksPerYAxis;
}

const Texture* gameTexture(GameTexture texture) {
    switch (texture) {
        case GAME_TEXTURE_BOX:
            return gBoxTexture;
        case GAME_TEXTURE_PLAYER:
            return gPlayerTexture;
        case GAME_TEXTURE_ENEMY:
            return gEnemyTexture;
        case GAME_TEXTURE_GEM:
            return gGemTexture;
    }
    return nullptr; // not gonna happen
}

void gameProcessInput(const SDL_Keycode* nullable keycode) {
    if (keycode == nullptr) return;

    const int
        playerPositionX = gameLevelPlayerPositionX(gGameLevel),
        playerPositionY = gameLevelPlayerPositionY(gGameLevel);

    switch (*keycode) {
        case SDLK_w:
            if (gCameraOffsetY > 0)
                gCameraOffsetY--;
            gameLevelTryMovePlayer(gGameLevel, playerPositionX, playerPositionY - 1);
            break;
        case SDLK_a:
            if (gCameraOffsetX > 0)
                gCameraOffsetX--;
            gameLevelTryMovePlayer(gGameLevel, playerPositionX - 1, playerPositionY);
            break;
        case SDLK_s:
            if (gCameraOffsetY < GAME_LEVEL_FIELD_ROWS - gBlocksPerYAxis)
                gCameraOffsetY++;
            gameLevelTryMovePlayer(gGameLevel, playerPositionX, playerPositionY + 1);
            break;
        case SDLK_d:
            if (gCameraOffsetX < GAME_LEVEL_FIELD_COLUMNS - gBlocksPerXAxis)
                gCameraOffsetX++;
            gameLevelTryMovePlayer(gGameLevel, playerPositionX + 1, playerPositionY);
            break;
    }
}

void gameUpdate(int deltaTime) {

}

void gameRender(void) {
    gameLevelDraw(gGameLevel, gCameraOffsetX, gCameraOffsetY, gSpriteRenderer);
}

void gameClean(void) {
    gameLevelDestroy(gGameLevel);

    compoundShaderDestroy(gSpriteShader);

    spriteRendererDestroy(gSpriteRenderer);

    textureDestroy(gBoxTexture);
    textureDestroy(gPlayerTexture);
    textureDestroy(gEnemyTexture);
    textureDestroy(gGemTexture);
}
