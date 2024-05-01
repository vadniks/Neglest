
#pragma once

#include "defs.h"
#include <SDL2/SDL.h>

extern const int GAME_BLOCK_SIZE, GAME_WIDTH, GAME_HEIGHT;

void gameInit(void);
void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime);
void gameUpdate(int deltaTime);
void gameRender(void);
void gameClean(void);
