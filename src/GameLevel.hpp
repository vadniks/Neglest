
#pragma once

#include "GameObject.hpp"
#include "SpriteRenderer.hpp"
#include <vector>
#include <string>
#include <memory>

class GameLevel {
private:
    std::vector<GameObject> mBricks;
public:
    GameLevel(const std::string& file, unsigned levelWidth, unsigned levelHeight);
    ~GameLevel();
    void draw(const std::shared_ptr<SpriteRenderer>& renderer);
    bool completed();
    std::vector<GameObject> bricks();
private:
    void init(std::vector<std::vector<unsigned>>&& tileData, unsigned levelWidth, unsigned levelHeight);
};
