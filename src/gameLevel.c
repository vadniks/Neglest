
#include "gameLevel.h"
#include <SDL2/SDL.h>

struct GameLevel {
    GameLevelEntity* field;
    int gBlocksPerXAxis, gBlocksPerYAxis;
};

static const int FIELD_ROWS = 50, FIELD_COLUMNS = 50;

GameLevel* gameLevelCreate(int gBlocksPerXAxis, int gBlocksPerYAxis, int which) {
    GameLevel* level = SDL_malloc(sizeof *level);
    level->gBlocksPerXAxis = gBlocksPerXAxis;
    level->gBlocksPerYAxis = gBlocksPerYAxis;

    level->field = SDL_malloc(FIELD_ROWS * FIELD_COLUMNS * sizeof(GameLevelEntity));
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLUMNS; j++)
            level->field[i * j] = GAME_LEVEL_ENTITY_BOX;
    }

    return level;
}

void gameLevelDestroy(GameLevel* level) {
    SDL_free(level->field);
    SDL_free(level);
}

void gameLevelDraw(const GameLevel* level, const SpriteRenderer* renderer) {
//    for (int i = 0; i < level->gBlocksPerXAxis; i++) {
//        for (int j = 0; j < level->gBlocksPerYAxis; j++) {
//            const Texture* nullable texture = nullptr;
//            vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
//
//            switch (level->field[i * j]) {
//                case GAME_LEVEL_ENTITY_EMPTY:
//                    texture = nullptr;
//                    break;
//                case GAME_LEVEL_ENTITY_BOX:
//                    texture = gBoxTexture;
//                    SDL_memcpy(color, (vec4) {0.4f, 0.5f, 0.5f, 1.0f}, 4 * sizeof(float));
//                    break;
//                case GAME_LEVEL_ENTITY_PLAYER:
//                    texture = gPlayerTexture;
//                    break;
//                case GAME_LEVEL_ENTITY_ENEMY:
//                    texture = gEnemyTexture;
//                    break;
//                case GAME_LEVEL_ENTITY_GEM:
//                    texture = gGemTexture;
//                    break;
//            }
//
//            if (texture == nullptr) continue;
//
//            spriteRendererDraw(
//                gSpriteRenderer,
//                texture,
//                (vec2) {(float) (GAME_BLOCK_SIZE * i), (float) (GAME_BLOCK_SIZE * j)},
//                (vec2) {(float) GAME_BLOCK_SIZE, (float) GAME_BLOCK_SIZE},
//                0.0f,
//                0.0f,
//                0.0f,
//                color
//            );
//        }
//    }
}

bool gameLevelCompleted(const GameLevel* level) {
    return false;
}
