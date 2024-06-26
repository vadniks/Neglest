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
#include "shapeRenderer.h"
#include "gameLevel.h"
#include <cglm/cam.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int GAME_BLOCK_SIZE = 50, GAME_WINDOW_WIDTH = GAME_BLOCK_SIZE * 32, GAME_WINDOW_HEIGHT = GAME_BLOCK_SIZE * 18;
const int LEVELS = 2;
const int RIGHT_PADDING = GAME_BLOCK_SIZE * 5;
static int gBlocksPerXAxis = 0, gBlocksPerYAxis = 0; // columns, rows
static CompoundShader* gSpriteShader = nullptr;
static SpriteRenderer* gSpriteRenderer = nullptr;
static CompoundShader* gShapeShader = nullptr;
static ShapeRenderer* gShapeRenderer = nullptr;
static Texture* gBoxTexture = nullptr;
static Texture* gPlayerTexture = nullptr;
static Texture* gEnemyTexture = nullptr;
static Texture* gGemTexture = nullptr;
static int gCurrentLevel = 0;
static GameLevel* nullable gGameLevel = nullptr;
static int gCameraOffsetX = 0, gCameraOffsetY = 0;
static TTF_Font* gSmallFont = nullptr;
static TTF_Font* gBigFont = nullptr;
static int gMouseX = 0, gMouseY = 0;
static bool gMouseButtonPressed = false;
static unsigned gStartMillis = 0, gFinishMillis = 0;

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
    gBlocksPerXAxis = (GAME_WINDOW_WIDTH - RIGHT_PADDING) / GAME_BLOCK_SIZE;
    gBlocksPerYAxis = GAME_WINDOW_HEIGHT / GAME_BLOCK_SIZE;

    mat4 projection;
    glm_ortho(0.0f, (float) GAME_WINDOW_WIDTH, (float) GAME_WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);

    gSpriteShader = compoundShaderCreate("shaders/spriteVertex.glsl", "shaders/spriteFragment.glsl");
    compoundShaderUse(gSpriteShader);
    compoundShaderSetMat4(gSpriteShader, "projection", projection);
    compoundShaderSetInt(gSpriteShader, "sprite", 0);

    gShapeShader = compoundShaderCreate("shaders/shapeVertex.glsl", "shaders/shapeFragment.glsl");
    compoundShaderUse(gShapeShader);
    compoundShaderSetMat4(gShapeShader, "projection", projection);

    gShapeRenderer = shapeRendererCreate(gShapeShader);

    gSpriteRenderer = spriteRendererCreate(gSpriteShader);
    gBoxTexture = loadTextureAndConvertFormat("res/box.png");
    gPlayerTexture = loadTextureAndConvertFormat("res/player_a.png");
    gEnemyTexture = loadTextureAndConvertFormat("res/enemy_a.png");
    gGemTexture = loadTextureAndConvertFormat("res/gem.png");

    gSmallFont = TTF_OpenFont("res/Roboto-Regular.ttf", 20);
    gBigFont = TTF_OpenFont("res/Roboto-Regular.ttf", 40);

    gStartMillis = SDL_GetTicks();
    gGameLevel = gameLevelCreate(gCurrentLevel);
}

int gameBlocksPerXAxis(void) { return gBlocksPerXAxis; }

int gameBlocksPerYAxis(void) { return gBlocksPerYAxis; }

int gameCurrentLevel(void) { return gCurrentLevel; }

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

void gameProcessKeyboardInput(const SDL_Keycode* nullable keycode) {
    if (gGameLevel == nullptr) return;
    if (keycode == nullptr) return;

    const int
        playerPositionX = gameLevelPlayerPositionX(gGameLevel),
        playerPositionY = gameLevelPlayerPositionY(gGameLevel);

    switch (*keycode) {
        case SDLK_w:
            if (gameLevelTryMovePlayer(gGameLevel, playerPositionX, playerPositionY - 1) && gCameraOffsetY > 0)
                gCameraOffsetY--;
            break;
        case SDLK_a:
            if (gameLevelTryMovePlayer(gGameLevel, playerPositionX - 1, playerPositionY) && gCameraOffsetX > 0)
                gCameraOffsetX--;
            break;
        case SDLK_s:
            if (gameLevelTryMovePlayer(gGameLevel, playerPositionX, playerPositionY + 1) && gCameraOffsetY < GAME_LEVEL_FIELD_ROWS - gBlocksPerYAxis)
                gCameraOffsetY++;
            break;
        case SDLK_d:
            if (gameLevelTryMovePlayer(gGameLevel, playerPositionX + 1, playerPositionY) && gCameraOffsetX < GAME_LEVEL_FIELD_COLUMNS - gBlocksPerXAxis)
                gCameraOffsetX++;
            break;
    }
}

void gameProcessMouseMotion(int x, int y) {
    gMouseX = x;
    gMouseY = y;
}

void gameProcessMouseButton(bool pressed) { gMouseButtonPressed = pressed; }

void gameUpdate(void) {
    if (gGameLevel == nullptr)
        return;
    gameLevelUpdate(gGameLevel);
}

static void drawText(TTF_Font* font, const vec2 position, const char* text, const vec4 color) {
    SDL_Surface* xSurface = TTF_RenderUTF8_Blended(font, text, (SDL_Color) {255, 255, 255, 255});
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(xSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(xSurface);

    Texture* texture = textureCreate(surface->w, surface->h, surface->pixels);
    spriteRendererDraw(gSpriteRenderer, texture, position, (vec2) {(float) surface->w, (float) surface->h}, 0.0f, 0.0f, 0.0f, color);

    SDL_FreeSurface(surface);
    textureDestroy(texture);
}

static void drawCollectedGems(void) {
    char text[16] = "Gems: ";
    SDL_itoa(gameLevelCollectedGems(gGameLevel), text + 6, 10);
    drawText(
        gSmallFont,
        (vec2) {(float) (GAME_WINDOW_WIDTH - RIGHT_PADDING + 5), (float) GAME_BLOCK_SIZE},
        text,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );
}

static void drawTotalGems(void) {
    char text[16] = "Total: ";
    SDL_itoa(gameLevelTotalGems(gGameLevel), text + 7, 10);
    drawText(
        gSmallFont,
        (vec2) {(float) (GAME_WINDOW_WIDTH - RIGHT_PADDING + 5), (float) GAME_BLOCK_SIZE * 1.5f},
        text,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );
}

static void drawCurrentLevel(void) {
    char text[32];
    assert(SDL_snprintf(text, sizeof text, "Current level: %d", gCurrentLevel) > 0);

    drawText(
        gSmallFont,
        (vec2) {(float) (GAME_WINDOW_WIDTH - RIGHT_PADDING + 5), (float) GAME_BLOCK_SIZE * 2.0f},
        text,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );
}

static void drawTime(void) {
    char text[64];
    assert(SDL_snprintf(text, sizeof text, "Time: %u ms", SDL_GetTicks() - gStartMillis) > 0);

    drawText(
        gSmallFont,
        (vec2) {(float) (GAME_WINDOW_WIDTH - RIGHT_PADDING + 5), (float) GAME_BLOCK_SIZE * 2.5f},
        text,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );
}

static void drawFinish(void) {
    char text[64];
    assert(SDL_snprintf(text, sizeof text, "Finished, %u ms!", gFinishMillis - gStartMillis));

    int w, h;
    TTF_SizeUTF8(gBigFont, text, &w, &h);

    drawText(
        gBigFont,
        (vec2) {(float) GAME_WINDOW_WIDTH / 2.0f - (float) w / 2.0f, (float) GAME_WINDOW_HEIGHT / 2.0f - (float) h / 2.0f},
        text,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );
}

static void restart(void) {
    gCameraOffsetX = 0;
    gCameraOffsetY = 0;
    gCurrentLevel = 0;
    gStartMillis = SDL_GetTicks();
    gFinishMillis = 0;
    gGameLevel = gameLevelCreate(0);
}

static void drawRestartButton(void) {
    const char* text = "Restart";

    int w, h;
    TTF_SizeUTF8(gSmallFont, text, &w, &h);

    const vec2 textPosition = {(float) GAME_WINDOW_WIDTH / 2.0f - (float) w / 2.0f, (float) GAME_WINDOW_HEIGHT / 2.0f + (float) h * 2.0f};

    const bool mouseHovered =
        gMouseX >= (int) textPosition[0] && gMouseX <= (int) textPosition[0] + w + 5 &&
        gMouseY >= (int) textPosition[1] && gMouseY <= (int) textPosition[1] + h + 5;

    if (mouseHovered && gMouseButtonPressed) {
        gMouseButtonPressed = false;
        restart();
        return;
    }

    drawText(
        gSmallFont,
        textPosition,
        text,
        (vec4) {1.0f, 1.0f, 1.0f, mouseHovered ? 0.5f : 1.0f}
    );

    shapeRendererDrawRectangle(
        gShapeRenderer,
        (vec2) {(float) GAME_WINDOW_WIDTH / 2.0f, (float) GAME_WINDOW_HEIGHT / 2.0f + (float) h * 2.5f},
        (vec2) {(float) w + 5.0f, (float) h + 5.0f},
        0.0f,
        0.0f,
        0.0f,
        (vec4) {1.0f, 1.0f, 1.0f, mouseHovered ? 0.5f : 1.0f},
        false
    );
}

void gameRender(void) {
    if (gGameLevel == nullptr) {
        if (gFinishMillis == 0)
            gFinishMillis = SDL_GetTicks();
        drawFinish();
        drawRestartButton();
        return;
    }

    gameLevelDraw(gGameLevel, gCameraOffsetX, gCameraOffsetY, gSpriteRenderer);

    shapeRendererDrawLine(
        gShapeRenderer,
        (vec2) {(float) (GAME_WINDOW_WIDTH - RIGHT_PADDING), (float) GAME_WINDOW_HEIGHT / 2.0f},
        (vec2) {(float) GAME_WINDOW_HEIGHT, 1.0f},
        90.0f,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );

    drawCollectedGems();
    drawTotalGems();
    drawCurrentLevel();
    drawTime();
}

void gameChangeLevel(void) {
    gameLevelDestroy(gGameLevel);

    gCameraOffsetX = 0;
    gCameraOffsetY = 0;

    if (++gCurrentLevel < LEVELS)
        gGameLevel = gameLevelCreate(gCurrentLevel);
    else
        gGameLevel = nullptr;
}

void gameClean(void) {
    if (gGameLevel != nullptr)
        gameLevelDestroy(gGameLevel);

    TTF_CloseFont(gSmallFont);
    TTF_CloseFont(gBigFont);

    compoundShaderDestroy(gSpriteShader);
    compoundShaderDestroy(gShapeShader);

    spriteRendererDestroy(gSpriteRenderer);
    shapeRendererDestroy(gShapeRenderer);

    textureDestroy(gBoxTexture);
    textureDestroy(gPlayerTexture);
    textureDestroy(gEnemyTexture);
    textureDestroy(gGemTexture);
}
