
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

const int GAME_BLOCK_SIZE = 50, GAME_WINDOW_WIDTH = GAME_BLOCK_SIZE * 32, GAME_WINDOW_HEIGHT = GAME_BLOCK_SIZE * 18;
static const int FIELD_ROWS = 100, FIELD_COLUMNS = 100;
//         columns              rows
static int gBlocksPerXAxis = 0, gBlocksPerYAxis = 0;
static CompoundShader* gSpriteShader = nullptr;
static SpriteRenderer* gSpriteRenderer = nullptr;
static Texture* gBoxTexture = nullptr;
static Texture* gPlayerTexture = nullptr;
static Texture* gEnemyTexture = nullptr;
static Texture* gGemTexture = nullptr;
static Entity* gField = nullptr;

static Texture* loadTextureAndConvertFormat(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    assert(surface != nullptr);

    SDL_Surface* xSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);

    Texture* texture = textureCreate(xSurface->w, xSurface->h, xSurface->pixels);
    SDL_FreeSurface(xSurface);
    return texture;
}

void gameInit(void) {
    gBlocksPerXAxis = GAME_WINDOW_WIDTH / GAME_BLOCK_SIZE;
    gBlocksPerYAxis = GAME_WINDOW_HEIGHT / GAME_BLOCK_SIZE;

    mat4 projection;
    glm_ortho(0.0f, (float) GAME_WINDOW_WIDTH, (float) GAME_WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);

    gSpriteShader = compoundShaderCreate("shaders/spriteVertex.glsl", "shaders/spriteFragment.glsl");
    compoundShaderUse(gSpriteShader);
    compoundShaderSetMat4(gSpriteShader, "projection", projection);
    compoundShaderSetInt(gSpriteShader, "sprite", 0);

    gSpriteRenderer = spriteRendererCreate(gSpriteShader);
    gBoxTexture = loadTextureAndConvertFormat("res/box.png");
    gPlayerTexture = loadTextureAndConvertFormat("res/player_a.png");
    gEnemyTexture = loadTextureAndConvertFormat("res/enemy_a.png");
    gGemTexture = loadTextureAndConvertFormat("res/gem.png");

    gField = SDL_malloc(FIELD_ROWS * FIELD_COLUMNS * sizeof(Entity));
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLUMNS; j++)
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
                (vec2) {(float) (GAME_BLOCK_SIZE * i), (float) (GAME_BLOCK_SIZE * j)},
                (vec2) {(float) GAME_BLOCK_SIZE, (float) GAME_BLOCK_SIZE},
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
