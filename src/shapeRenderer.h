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

#include "compoundShader.h"
#include <cglm/types.h>

struct ShapeRenderer;
typedef struct ShapeRenderer ShapeRenderer;

ShapeRenderer* shapeRendererCreate(const CompoundShader* shader);
void shapeRendererDestroy(ShapeRenderer* renderer);
void shapeRendererDrawTriangle(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    float rotationX,
    float rotationY,
    float rotationZ,
    const vec4 color,
    bool fill
);
void shapeRendererDrawRectangle(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    float rotationX,
    float rotationY,
    float rotationZ,
    const vec4 color,
    bool fill
);
void shapeRendererDrawLine(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    float rotationZ,
    const vec4 color
);
// draw point
