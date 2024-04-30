
#pragma once

#include "defs.h"
#include <SDL2/SDL.h>

void gameInit(int blockSize, int width, int height);
void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime);
void gameUpdate(int deltaTime);
void gameRender(void);
void gameClean(void);
