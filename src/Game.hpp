
#pragma once

#include "GameState.hpp"
#include <array>

class Game {
public:
    GameState state;
    unsigned width, height;
    std::array<bool, 1024> keys;

    Game(unsigned width, unsigned height);
    ~Game();
    void init();
    void processInput();
    void update();
    void render();
};
