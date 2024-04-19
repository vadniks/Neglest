
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include "BallObject.hpp"
#include <glm/ext/matrix_clip_space.hpp>

static const std::string IMAGE = "image";
static const std::string PROJECTION = "projection";

static std::shared_ptr<SpriteRenderer> gRenderer;

static const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
static const float PLAYER_VELOCITY = 25.0f;
static const glm::vec2 INITIAL_BALL_VELOCITY(15.0f, -15.0f);
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
