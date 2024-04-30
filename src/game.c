
#include "game.h"
#include "compoundShader.h"
#include "spriteRenderer.h"
#include "texture.h"
#include <cglm/cam.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef enum {
    ENTITY_EMPTY,
    ENTITY_BOX,
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_GEM
} Entity;

static int gBlockSize = 0, gWidth = 0, gHeight = 0, gBlocksPerXAxis = 0, gBlocksPerYAxis;
static CompoundShader* gSpriteShader = nullptr;
static SpriteRenderer* gSpriteRenderer = nullptr;
static Texture* gBoxTexture = nullptr;
static Texture* gPlayerTexture = nullptr;
static Texture* gEnemyTexture = nullptr;
static Texture* gGemTexture = nullptr;
static Entity* gField = nullptr;

void gameInit(int blockSize, int width, int height) {
    gBlockSize = blockSize;
    gWidth = width;
    gHeight = height;
    gBlocksPerXAxis = gWidth / gBlockSize;
    gBlocksPerYAxis = gHeight / gBlockSize;

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

    gField = SDL_malloc(gBlocksPerXAxis * gBlocksPerYAxis * sizeof(Entity));
    for (int i = 0; i < gBlocksPerXAxis; i++) {
        for (int j = 0; j < gBlocksPerYAxis; j++)
            gField[i * j] = ENTITY_BOX;
    }
}

void gameProcessInput(SDL_Keycode* nullable keycode, int deltaTime) {

}

void gameUpdate(int deltaTime) {

}

void gameRender(void) {
    for (int i = 0; i < gBlocksPerXAxis; i++) {
        for (int j = 0; j < gBlocksPerYAxis; j++) {
            const Texture* nullable texture = nullptr;

            switch (gField[i * j]) {
                case ENTITY_EMPTY:
                    texture = nullptr;
                    break;
                case ENTITY_BOX:
                    texture = gBoxTexture;
                    break;
                case ENTITY_PLAYER:
                    texture = gPlayerTexture;
                    break;
                case ENTITY_ENEMY:
                    texture = gEnemyTexture;
                    break;
                case ENTITY_GEM:
                    texture = gGemTexture;
                    break;
            }

            if (texture == nullptr) continue;

            spriteRendererDraw(
                gSpriteRenderer,
                texture,
                (vec2) {(float) (gBlockSize * i), (float) (gBlockSize * j)},
                (vec2) {(float) gBlockSize, (float) gBlockSize},
                0.0f,
                0.0f,
                0.0f,
                (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
            );
        }
    }
}

void gameClean(void) {
    compoundShaderDestroy(gSpriteShader);

    spriteRendererDestroy(gSpriteRenderer);

    textureDestroy(gBoxTexture);
    textureDestroy(gPlayerTexture);
    textureDestroy(gEnemyTexture);
    textureDestroy(gGemTexture);

    SDL_free(gField);
}
