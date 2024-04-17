
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <glm/ext/matrix_clip_space.hpp>

static const std::string IMAGE = "image";
static const std::string PROJECTION = "projection";

static std::shared_ptr<SpriteRenderer> gRenderer;

static const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
static const float PLAYER_VELOCITY = 25.0f;

static GameObject* gPlayer;

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
}

void Game::init() {
    auto shader = ResourceManager::instance()->loadShader(
        "shaders/spriteVertex.glsl",
        "shaders/spriteFragment.glsl",
        "sprite"
    );

    glm::mat4 proj = glm::ortho(
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

    ResourceManager::instance()->loadTexture("res/background.jpg", false, "background");
    ResourceManager::instance()->loadTexture("res/block.png", false, "block");
    ResourceManager::instance()->loadTexture("res/block_solid.png", false, "blockSolid");
    ResourceManager::instance()->loadTexture("res/paddle.png", true, "paddle");

    mLevels.emplace_back("res/one.lvl", mWidth, mHeight / 2);
    mLevel = 0;

    auto playerPos = glm::vec2(static_cast<float>(mWidth) / 2.0f - PLAYER_SIZE.x / 2.0f, static_cast<float>(mHeight) - PLAYER_SIZE.y);
    gPlayer = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::instance()->getTexture("paddle"));
}

void Game::processInput(const SDL_Keycode* keyCode) {
    if (keyCode == nullptr) return;
    const glm::vec2 pos = gPlayer->position();
    switch (*keyCode) {
        case SDLK_a:
            if (pos.x >= 0.0f)
                gPlayer->setPosition(glm::vec2(pos.x - PLAYER_VELOCITY, pos.y));
            break;
        case SDLK_d:
            if (pos.x <= static_cast<float>(mWidth) - gPlayer->size().x)
                gPlayer->setPosition(glm::vec2(pos.x + PLAYER_VELOCITY, pos.y));
            break;
    }
}

void Game::update() {

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
}
