
#include "gameLevel.h"
#include "game.h"
#include <assert.h>
#include <SDL2/SDL.h>

struct GameLevel {
    GameLevelEntity* field;
};

static const int FIELD_ROWS = 50, FIELD_COLUMNS = 50;

GameLevel* gameLevelCreate(int which) {
    const int levelNameMaxSize = 64;
    char levelName[levelNameMaxSize];
    assert(SDL_snprintf(levelName, levelNameMaxSize, "res/level%d.txt", which) > 0);

    char data[FIELD_ROWS * FIELD_COLUMNS];

    SDL_RWops* file = SDL_RWFromFile(levelName, "r");
    assert(file != nullptr);
    assert(SDL_RWread(file, data, 1, sizeof data) > 0);
    SDL_RWclose(file);

    GameLevel* level = SDL_malloc(sizeof *level);
    level->field = SDL_malloc(FIELD_ROWS * FIELD_COLUMNS * sizeof(GameLevelEntity));

    int xChar = 0;
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLUMNS; j++) {
            switch (data[xChar++]) {
                case 'b':
                    level->field[i * j] = GAME_LEVEL_ENTITY_BOX;
                    break;
                case '.':
                    level->field[i * j] = GAME_LEVEL_ENTITY_EMPTY;
                    break;
                case 'e':
                    level->field[i * j] = GAME_LEVEL_ENTITY_ENEMY;
                    break;
                case 'g':
                    level->field[i * j] = GAME_LEVEL_ENTITY_GEM;
                    break;
                case '\n':
                    continue;
                case 0:
                    break;
                default:
                    assert(false);
            }
        }
    }

    return level;
}

void gameLevelDestroy(GameLevel* level) {
    SDL_free(level->field);
    SDL_free(level);
}

void gameLevelDraw(int cameraOffsetX, int cameraOffsetY, const GameLevel* level, const SpriteRenderer* renderer) {
    for (int i = 0; i < gameBlocksPerYAxis(); i++) {
        for (int j = 0; j < gameBlocksPerXAxis(); j++) {
            const Texture* nullable texture = nullptr;
            vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

            switch (level->field[(i + cameraOffsetY) * (j + cameraOffsetX)]) {
                case GAME_LEVEL_ENTITY_EMPTY:
                    texture = nullptr;
                    break;
                case GAME_LEVEL_ENTITY_BOX:
                    texture = gameTexture(GAME_TEXTURE_BOX);
                    SDL_memcpy(color, (vec4) {0.3f, 0.5f, 0.7f, 1.0f}, 4 * sizeof(float));
                    break;
                case GAME_LEVEL_ENTITY_PLAYER:
                    texture = gameTexture(GAME_TEXTURE_PLAYER);
                    break;
                case GAME_LEVEL_ENTITY_ENEMY:
                    texture = gameTexture(GAME_TEXTURE_ENEMY);
                    break;
                case GAME_LEVEL_ENTITY_GEM:
                    texture = gameTexture(GAME_TEXTURE_GEM);
                    break;
            }

            if (texture == nullptr) continue;

            spriteRendererDraw(
                renderer,
                texture,
                (vec2) {(float) (GAME_BLOCK_SIZE * j), (float) (GAME_BLOCK_SIZE * i)},
                (vec2) {(float) GAME_BLOCK_SIZE, (float) GAME_BLOCK_SIZE},
                0.0f,
                0.0f,
                0.0f,
                color
            );
        }
    }
}

bool gameLevelCompleted(const GameLevel* level) {
    return false;
}
