
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
static Texture* gBoxTexture = nullptr;
static Texture* gPlayerTexture = nullptr;
static Texture* gEnemyTexture = nullptr;
static Texture* gGemTexture = nullptr;

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

    SDL_Surface* boxSurface = IMG_Load("res/box.png");
    SDL_Surface* xBoxSurface = SDL_ConvertSurfaceFormat(boxSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(boxSurface);
    gBoxTexture = textureCreate(xBoxSurface->w, xBoxSurface->h, xBoxSurface->pixels);
    SDL_FreeSurface(xBoxSurface);

    SDL_Surface* playerSurface = IMG_Load("res/player_a.png");
    SDL_Surface* xPlayerSurface = SDL_ConvertSurfaceFormat(playerSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(playerSurface);
    gPlayerTexture = textureCreate(xPlayerSurface->w, xPlayerSurface->h, xPlayerSurface->pixels);
    SDL_FreeSurface(xPlayerSurface);

    SDL_Surface* enemySurface = IMG_Load("res/enemy_a.png");
    SDL_Surface* xEnemySurface = SDL_ConvertSurfaceFormat(enemySurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(enemySurface);
    gEnemyTexture = textureCreate(xEnemySurface->w, xEnemySurface->h, xEnemySurface->pixels);
    SDL_FreeSurface(xEnemySurface);

    SDL_Surface* gemSurface = IMG_Load("res/gem.png");
    SDL_Surface* xGemSurface = SDL_ConvertSurfaceFormat(gemSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(gemSurface);
    gGemTexture = textureCreate(xGemSurface->w, xGemSurface->h, xGemSurface->pixels);
    SDL_FreeSurface(xGemSurface);
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
                gBoxTexture,
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
    textureDestroy(gBoxTexture);
    textureDestroy(gPlayerTexture);
    textureDestroy(gEnemyTexture);
    textureDestroy(gGemTexture);
}
