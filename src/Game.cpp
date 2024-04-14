
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <glm/ext/matrix_clip_space.hpp>

static SpriteRenderer* renderer = nullptr;

Game::Game(unsigned width, unsigned height) :
    state(GameState::GAME_MENU),
    width(width),
    height(height),
    keys()
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
        static_cast<float>(width),
        static_cast<float>(height),
        0.0f,
        -1.0f,
        1.0f
    );

    shader->use();
    shader->setValue("image", 0);
    shader->setValue("projection", proj);

    renderer = new SpriteRenderer(shader);

    ResourceManager::instance()->loadTexture("res/awesomeface.png", true, "face");
}

void Game::processInput() {

}

void Game::update() {

}

void Game::render() {
    renderer->draw(
        ResourceManager::instance()->getTexture("face"),
        glm::vec2(200.0f, 200.0f),
        glm::vec2(300.0f, 400.0f),
        45.0f,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}
