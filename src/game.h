
#pragma once

#include "defs.h"
#include <SDL2/SDL.h>

typedef enum {
    GAME_BLOCK_SIZE = 50
} GameConsts;

void gameInit(int blockSize, int width, int height);
void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime);
void gameUpdate(int deltaTime);
void gameRender(void);
void gameClean(void);
