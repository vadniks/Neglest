
#include "spriteRenderer.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>

struct SpriteRenderer {
    const CompoundShader* shader;
    unsigned vao, vbo;
};

SpriteRenderer* spriteRendererCreate(const CompoundShader* shader) {
    SpriteRenderer* renderer = SDL_malloc(sizeof *renderer);
    renderer->shader = shader;
    renderer->vao = 0;
    renderer->vbo = 0;

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, &(renderer->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &(renderer->vao));
    glBindVertexArray(renderer->vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return renderer;
}

void spriteRendererDestroy(SpriteRenderer* renderer) {
    glDeleteBuffers(1, &(renderer->vbo));
    glDeleteVertexArrays(1, &(renderer->vao));
    SDL_free(renderer);
}

void spriteRendererDraw(
    const SpriteRenderer* renderer,
    const Texture* texture,
    const vec2 position,
    const vec2 size,
    float rotation,
    const vec4 color
) {
    compoundShaderUse(renderer->shader);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], 0.0f});

    glm_translate(model, (vec3) {0.5f * size[0], 0.5f * size[1], 0.0f});
    glm_rotate(model, glm_rad(rotation), (vec3) {0.0f, 0.0f, 1.0f});
    glm_translate(model, (vec3) {-0.5f * size[0], -0.5f * size[1], 0.0f});

    glm_scale(model, (vec3) {size[0], size[1], 1.0f});

    compoundShaderSetMat4(renderer->shader, "model", model);
    compoundShaderSetVec4(renderer->shader, "spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    textureBind(texture);

    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
