
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include "BallObject.hpp"
#include <tuple>
#include <glm/ext/matrix_clip_space.hpp>

using Collision = std::tuple<bool, Direction, glm::vec2>;

static const std::string IMAGE = "image";
static const std::string PROJECTION = "projection";

static std::shared_ptr<SpriteRenderer> gRenderer;

static const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
static const float PLAYER_VELOCITY = 30.0f;
static const glm::vec2 INITIAL_BALL_VELOCITY(7.5f, -7.5f);
static const float BALL_RADIUS = 12.5f;

static GameObject* gPlayer;
static BallObject* gBall;

Game::Game(unsigned width, unsigned height) :
    mState(GameState::GAME_MENU),
    mWidth(width),
    mHeight(height)
{}

Game::~Game() {
    const auto useCount = static_cast<int>(gRenderer.use_count());
    for (int i = 0; i < useCount; i++)
        gRenderer.reset();
    delete gRenderer.get();

    delete gPlayer;
    delete gBall;
}

void Game::init() {
    const auto shader = ResourceManager::instance()->loadShader(
        "shaders/spriteVertex.glsl",
        "shaders/spriteFragment.glsl",
        "sprite"
    );

    const glm::mat4 proj = glm::ortho(
        0.0f,
        static_cast<float>(mWidth),
        static_cast<float>(mHeight),
        0.0f,
        -1.0f,
        1.0f
    );

    shader->use();
    shader->setValue(IMAGE, 0);
    shader->setValue(PROJECTION, proj);

    gRenderer.reset(new SpriteRenderer(shader));

    ResourceManager::instance()->loadTexture("res/awesomeface.png", true, "face");
    ResourceManager::instance()->loadTexture("res/background.jpg", false, "background");
    ResourceManager::instance()->loadTexture("res/block.png", false, "block");
    ResourceManager::instance()->loadTexture("res/block_solid.png", false, "blockSolid");
    ResourceManager::instance()->loadTexture("res/paddle.png", true, "paddle");

    mLevels.emplace_back("res/one.lvl", mWidth, mHeight / 2);
    mLevel = 0;

    const auto playerPos = glm::vec2(static_cast<float>(mWidth) / 2.0f - PLAYER_SIZE.x / 2.0f, static_cast<float>(mHeight) - PLAYER_SIZE.y);
    gPlayer = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::instance()->getTexture("paddle"));

    const glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    gBall = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::instance()->getTexture("face"));
}

void Game::processInput(const SDL_Keycode* keyCode) {
    if (keyCode == nullptr) return;

    const glm::vec2 playerPos = gPlayer->position();
    const glm::vec2 ballPos = gBall->position();

    switch (*keyCode) {
        case SDLK_a:
            if (playerPos.x >= 0.0f) {
                gPlayer->setPosition(glm::vec2(playerPos.x - PLAYER_VELOCITY, playerPos.y));
                if (gBall->stuck())
                    gBall->setPosition(glm::vec2(ballPos.x - PLAYER_VELOCITY, ballPos.y));
            }
            break;
        case SDLK_d:
            if (playerPos.x <= static_cast<float>(mWidth) - gPlayer->size().x) {
                gPlayer->setPosition(glm::vec2(playerPos.x + PLAYER_VELOCITY, playerPos.y));
                if (gBall->stuck())
                    gBall->setPosition(glm::vec2(ballPos.x + PLAYER_VELOCITY, ballPos.y));
            }
            break;
        case SDLK_SPACE:
            gBall->setStuck(false);
            break;
    }
}

void Game::update() {
    gBall->move(mWidth);
    doCollisions();
}

void Game::render() {
    gRenderer->draw(
        ResourceManager::instance()->getTexture("background"),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(mWidth, mHeight),
        0.0f
    );
    mLevels[mLevel].draw(gRenderer);
    gPlayer->draw(gRenderer);
    gBall->draw(gRenderer);
}

static bool checkCollision(GameObject* one, GameObject* two) {
    const bool collisionX = one->position().x + one->size().x >= two->position().x && two->position().x + two->size().x >= one->position().x;
    const bool collisionY = one->position().y + one->size().y >= two->position().y && two->position().y + two->size().y >= one->position().y;
    return collisionX && collisionY;
}

static Direction vectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };

    float max = 0.0f;
    unsigned int best_match = -1;

    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);

        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }

    return (Direction)best_match;
}

static Collision checkCollision(BallObject* one, GameObject* two) {
    glm::vec2 center(one->position() + one->radius());

    glm::vec2 aabb_half_extents(two->size().x / 2.0f, two->size().y / 2.0f);
    glm::vec2 aabb_center(two->position().x + aabb_half_extents.x, two->position().y + aabb_half_extents.y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    if (glm::length(difference) <= one->radius())
        return std::make_tuple(true, vectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::doCollisions() {
    for (auto& box : mLevels[mLevel].bricks()) {
        if (!box.destroyed()) {
            Collision collision = checkCollision(gBall, &box);

            if (std::get<0>(collision)) {
                if (!box.solid())
                    box.setDestroyed(true);

                Direction direction = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);

                if (direction == LEFT || direction == RIGHT) {
                    gBall->setVelocity(glm::vec2(-(gBall->velocity().x), gBall->velocity().y));

                    float penetration = gBall->radius() - std::abs(diff_vector.x);
                    if (direction == LEFT)
                        gBall->setPosition(glm::vec2(gBall->position().x + penetration, gBall->position().y));
                    else
                        gBall->setPosition(glm::vec2(gBall->position().x - penetration, gBall->position().y));
                } else {
                    gBall->setVelocity(glm::vec2(gBall->velocity().x, -(gBall->velocity().y)));

                    float penetration = gBall->radius() - std::abs(diff_vector.y);
                    if (direction == UP)
                        gBall->setPosition(glm::vec2(gBall->position().x, gBall->position().y + penetration));
                    else
                        gBall->setPosition(glm::vec2(gBall->position().x, gBall->position().y - penetration));
                }
            }
        }
    }
}
