
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
void shapeRendererDrawHorizontalLine(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    const vec4 color
);
// draw point
