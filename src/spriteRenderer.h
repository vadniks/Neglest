
#pragma once

#include "compoundShader.h"
#include "texture.h"
#include <cglm/types.h>

struct SpriteRenderer;
typedef struct SpriteRenderer SpriteRenderer;

SpriteRenderer* spriteRendererCreate(const CompoundShader* shader);
void spriteRendererDestroy(SpriteRenderer* renderer);
void spriteRendererDraw(
    const SpriteRenderer* renderer,
    const Texture* texture,
    const vec2 position,
    const vec2 size,
    float rotation,
    const vec4 color
);
void spriteRendererDrawMirrored(
    const SpriteRenderer* renderer,
    const Texture* texture,
    const vec2 position,
    const vec2 size,
    float rotation,
    const vec4 color
);
