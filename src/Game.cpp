
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <glm/ext/matrix_clip_space.hpp>

static const std::string IMAGE = "image";
static const std::string PROJECTION = "projection";

static SpriteRenderer* renderer = nullptr;

Game::Game(unsigned width, unsigned height) :
    mState(GameState::GAME_MENU),
    mWidth(width),
    mHeight(height),
    mKeys()
{}

Game::~Game() {
    delete renderer;
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

    renderer = new SpriteRenderer(shader);

    ResourceManager::instance()->loadTexture("res/awesomeface.png", true, "face");
    ResourceManager::instance()->loadTexture("res/background.jpg", false, "background");
    ResourceManager::instance()->loadTexture("res/block.png", true, "block");
    ResourceManager::instance()->loadTexture("res/block_solid.png", true, "blockSolid");

    mLevels.emplace_back("res/one.lvl", mWidth, mHeight / 2);
    mLevel = 0;
}

void Game::processInput() {

}

void Game::update() {

}

void Game::render() {
    renderer->draw(
        ResourceManager::instance()->getTexture("background"),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(mWidth, mHeight),
        0.0f
    );
    mLevels[mLevel].draw(std::shared_ptr<SpriteRenderer>(renderer));
}
