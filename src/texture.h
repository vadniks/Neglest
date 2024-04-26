
#pragma once

#include "defs.h"

struct Texture;
typedef struct Texture Texture;

Texture* textureCreate(int width, int height, const byte* data);
void textureDestroy(Texture* texture);
void textureBind(const Texture* texture);
int textureWidth(const Texture* texture);
int textureHeight(const Texture* texture);
