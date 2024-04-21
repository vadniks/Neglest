
#include "GameLevel.hpp"
#include "ResourceManager.hpp"
#include <fstream>
#include <sstream>

GameLevel::GameLevel(const std::string& file, unsigned levelWidth, unsigned levelHeight) {
    unsigned tileCode;
    std::string line;
    std::ifstream stream(file);
    assert(stream);

    std::vector<std::vector<unsigned>> tileData;
    while (std::getline(stream, line)) {
        std::istringstream stringStream(line);
        std::vector<unsigned> row;

        while (stringStream >> tileCode)
            row.push_back(tileCode);
        tileData.push_back(std::move(row));
    }

    if (!tileData.empty())
        init(std::move(tileData), levelWidth, levelHeight);
}

GameLevel::~GameLevel() = default;

void GameLevel::draw(const std::shared_ptr<SpriteRenderer>& renderer) {
    for (auto& tile : mBricks)
        if (!tile.destroyed())
            tile.draw(renderer);
}

bool GameLevel::completed() {
    for (auto& tile : mBricks)
        if (!tile.solid() && !tile.destroyed())
            return false;
    return true;
}

std::vector<GameObject> GameLevel::bricks() {
    return mBricks;
}

void GameLevel::init(std::vector<std::vector<unsigned>>&& tileData, unsigned levelWidth, unsigned levelHeight) {
    unsigned height = tileData.size(), width = tileData[0].size();
    float unitWidth = static_cast<float>(levelWidth) / static_cast<float>(width);
    float unitHeight = static_cast<float>(levelHeight) / static_cast<float>(height);

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            glm::vec2 pos(unitWidth * static_cast<float>(x), unitHeight * static_cast<float>(y));
            glm::vec2 size(unitWidth, unitHeight);

            if (tileData[y][x] == 1) {
                GameObject obj(pos, size, ResourceManager::instance()->getTexture("blockSolid"), glm::vec4(0.8f, 0.8f, 0.7f, 1.0f));
                obj.setSolid(true);
                mBricks.push_back(std::move(obj));
            } else if (tileData[y][x] > 1) {
                auto color = glm::vec4(1.0f);

                switch (tileData[y][x]) {
                    case 2:
                        color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
                        break;
                    case 3:
                        color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
                        break;
                    case 4:
                        color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
                        break;
                    case 5:
                        color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
                        break;
                }

                mBricks.emplace_back(pos, size, ResourceManager::instance()->getTexture("block"), color);
            }
        }
    }
}
