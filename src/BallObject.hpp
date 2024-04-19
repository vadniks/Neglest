
#pragma once

#include "GameObject.hpp"
#include "Texture.hpp"
#include <memory>
#include <glm/glm.hpp>

class BallObject : public GameObject {
private:
    float mRadius;
    bool mStuck;
public:
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, const std::shared_ptr<Texture>& sprite);
    glm::vec2 move(unsigned windowWidth);
    void reset(glm::vec2 position, glm::vec2 velocity);
    float radius();
    bool stuck();
    void setStuck(bool stuck);
};
