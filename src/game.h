
#pragma once

#include "defs.h"
#include "texture.h"
#include <SDL2/SDL.h>

typedef enum {
    GAME_TEXTURE_BOX,
    GAME_TEXTURE_PLAYER,
    GAME_TEXTURE_ENEMY,
    GAME_TEXTURE_GEM
} GameTexture;

extern const int GAME_BLOCK_SIZE, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT;

void gameInit(void);
int gameBlocksPerXAxis(void);
int gameBlocksPerYAxis(void);
const Texture* gameTexture(GameTexture texture);
void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime);
void gameUpdate(int deltaTime);
void gameRender(void);
void gameClean(void);
