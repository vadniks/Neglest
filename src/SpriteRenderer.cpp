
#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader> shader) {
    const float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, (unsigned[1]) {mVbo});
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, (unsigned[1]) {mVao});
    glBindVertexArray(mVao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteBuffers(1, (unsigned[1]) {mVbo});
    glDeleteVertexArrays(1, (unsigned[1]) {mVao});
}

void SpriteRenderer::draw(
    std::shared_ptr<Texture> texture,
    glm::vec2 position,
    glm::vec2 size,
    float rotation,
    glm::vec3 color
) {

}
