
#include "BallObject.hpp"

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, const std::shared_ptr<Texture>& sprite) :
    GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec4(1.0f), velocity),
    mRadius(radius), mStuck(true)
{}

glm::vec2 BallObject::move(unsigned windowWidth) {
    if (mStuck) return mPosition;

    mPosition += mVelocity;

    if (mPosition.x <= 0.0f) {
        mVelocity.x = -mVelocity.x;
        mPosition.x = 0.0f;
    } else if (mPosition.x + mSize.x >= static_cast<float>(windowWidth)) {
        mVelocity.x = -mVelocity.x;
        mPosition.x = static_cast<float>(windowWidth) - mSize.x;
    }

    if (mPosition.y <= 0.0f) {
        mVelocity.y = -mVelocity.y;
        mPosition.y = 0.0f;
    }

    return mPosition;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {

}

float BallObject::radius() {
    return mRadius;
}

bool BallObject::stuck() {
    return mStuck;
}

void BallObject::setStuck(bool stuck) {
    mStuck = stuck;
}
