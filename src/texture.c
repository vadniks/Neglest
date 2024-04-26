
#include "texture.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

struct Texture {
    unsigned id;
    int width, height;
};

Texture* textureCreate(int width, int height, const byte* data) {
    Texture* texture = SDL_malloc(sizeof *texture);
    texture->id = 0;
    texture->width = width;
    texture->height = height;

    glGenTextures(1, &(texture->id));
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void textureDestroy(Texture* texture) {
    glDeleteTextures(1, &(texture->id));
    SDL_free(texture);
}

void textureBind(const Texture* texture)
{ glBindTexture(GL_TEXTURE_2D, texture->id); }

int textureWidth(const Texture* texture)
{ return texture->width; }

int textureHeight(const Texture* texture)
{ return texture->height; }
