
#include "SpriteRenderer.hpp"
#include <glm/ext/matrix_transform.hpp>

static const std::string MODEL = "model";
static const std::string SPRITE_COLOR = "spriteColor";

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader) {
    mShader = shader;
    mVao = 0;
    mVbo = 0;

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
    const std::shared_ptr<Texture>& texture,
    glm::vec2 position,
    glm::vec2 size,
    float rotation,
    glm::vec3 color
) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5*size.x, 0.5*size.y, 0.0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5*size.x, -0.5*size.y, 0.0));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    mShader->use();
    mShader->setValue(MODEL, model);
    mShader->setValue(SPRITE_COLOR, color);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(mVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
