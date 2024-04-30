
#include "game.h"
#include "compoundShader.h"
#include "spriteRenderer.h"
#include "texture.h"
#include <cglm/cam.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static int gBlockSize = 0, gWidth = 0, gHeight = 0;
static CompoundShader* gSpriteShader = nullptr;
static SpriteRenderer* gSpriteRenderer = nullptr;
static Texture* gFaceTexture = nullptr;

void gameInit(int blockSize, int width, int height) {
    gBlockSize = blockSize;
    gWidth = width;
    gHeight = height;

    mat4 projection;
    glm_ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f, projection);

    gSpriteShader = compoundShaderCreate("shaders/spriteVertex.glsl", "shaders/spriteFragment.glsl");
    compoundShaderUse(gSpriteShader);
    compoundShaderSetMat4(gSpriteShader, "projection", projection);
    compoundShaderSetInt(gSpriteShader, "sprite", 0);

    gSpriteRenderer = spriteRendererCreate(gSpriteShader);

    SDL_Surface* faceSurface = IMG_Load("res/box.png");
    SDL_Surface* xFaceSurface = SDL_ConvertSurfaceFormat(faceSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(faceSurface);
    gFaceTexture = textureCreate(xFaceSurface->w, xFaceSurface->h, xFaceSurface->pixels);
    SDL_FreeSurface(xFaceSurface);
}

void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime) {

}

void gameUpdate(int deltaTime) {

}

void gameRender(void) {
    for (int i = 0; i < gWidth / gBlockSize; i++)
        for (int j = 0; j < gHeight / gBlockSize; j++)
            spriteRendererDraw(
                gSpriteRenderer,
                gFaceTexture,
                (vec2) {(float) (gBlockSize * i), (float) (gBlockSize * j)},
                (vec2) {(float) gBlockSize, (float) gBlockSize},
                0.0f,
                0.0f,
                0.0f,
                (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
            );
}

void gameClean(void) {
    compoundShaderDestroy(gSpriteShader);
    spriteRendererDestroy(gSpriteRenderer);
    textureDestroy(gFaceTexture);
}
