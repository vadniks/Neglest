
#pragma once

#include "GameState.hpp"
#include "GameLevel.hpp"
#include <array>
#include <vector>

class Game {
private:
    GameState mState;
    unsigned mWidth, mHeight;
    std::array<bool, 1024> mKeys;
    std::vector<GameLevel> mLevels;
    unsigned mLevel;
public:
    Game(unsigned width, unsigned height);
    ~Game();
    void init();
    void processInput();
    void update();
    void render();
};
