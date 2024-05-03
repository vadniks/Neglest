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
void gameProcessInput(const SDL_Keycode* nullable keycode);
void gameUpdate(void);
void gameRender(void);
void gameChangeLevel(void);
void gameClean(void);
