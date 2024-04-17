
#pragma once

#include "GameState.hpp"
#include "GameLevel.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Game {
private:
    GameState mState;
    unsigned mWidth, mHeight;
    std::vector<GameLevel> mLevels;
    unsigned mLevel;
public:
    Game(unsigned width, unsigned height);
    ~Game();
    void init();
    void processInput(const SDL_Keycode* keyCode = nullptr);
    void update();
    void render();
};
