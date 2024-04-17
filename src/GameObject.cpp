
#include "GameObject.hpp"

GameObject::GameObject(
    glm::vec2 position,
    glm::vec2 size,
    const std::shared_ptr<Texture>& sprite,
    glm::vec4 color,
    glm::vec2 velocity
) :
    mPosition(position),
    mSize(size),
    mVelocity(velocity),
    mColor(color),
    mRotation(0.0f),
    mSolid(false),
    mDestroyed(false),
    mSprite(sprite)
{}

GameObject::~GameObject() = default;

void GameObject::draw(const std::shared_ptr<SpriteRenderer>& renderer) {
    renderer->draw(mSprite, mPosition, mSize, mRotation, mColor);
}

bool GameObject::solid() {
    return mSolid;
}

void GameObject::setSolid(bool solid) {
    mSolid = solid;
}

bool GameObject::destroyed() {
    return mDestroyed;
}
