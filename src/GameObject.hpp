
#pragma once

#include "Texture.hpp"
#include "SpriteRenderer.hpp"
#include <glm/glm.hpp>
#include <memory>

class GameObject {
protected:
    glm::vec2 mPosition, mSize, mVelocity;
    glm::vec4 mColor;
    float mRotation;
    bool mSolid;
    bool mDestroyed;
    std::shared_ptr<Texture> mSprite;
public:
    GameObject(
        glm::vec2 position,
        glm::vec2 size,
        const std::shared_ptr<Texture>& sprite,
        glm::vec4 color = glm::vec4(1.0f),
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f)
    );
    virtual ~GameObject();
    virtual void draw(const std::shared_ptr<SpriteRenderer>& renderer);
    glm::vec2 position();
    void setPosition(glm::vec2 pos);
    glm::vec2 size();
    bool solid();
    void setSolid(bool solid);
    bool destroyed();
    void setDestroyed(bool destroyed);
};
