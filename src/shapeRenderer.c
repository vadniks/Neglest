
#include "shapeRenderer.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cglm/affine.h>

struct ShapeRenderer {
    const CompoundShader* shader;
    unsigned vbo, ebo, vao;
};

ShapeRenderer* shapeRendererCreate(const CompoundShader* shader) {
    ShapeRenderer* renderer = SDL_malloc(sizeof *renderer);
    renderer->shader = shader;
    glGenBuffers(1, &(renderer->vbo));
    glGenBuffers(1, &(renderer->ebo));
    glGenVertexArrays(1, &((renderer->vao)));
    return renderer;
}

void shapeRendererDestroy(ShapeRenderer* renderer) {
    glDeleteBuffers(1, &(renderer->vbo));
    glDeleteBuffers(1, &(renderer->ebo));
    glDeleteVertexArrays(1, &((renderer->vao)));
    SDL_free(renderer);
}

void shapeRendererDrawTriangle(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    float rotationX,
    float rotationY,
    float rotationZ,
    const vec4 color,
    bool fill
) {
    glBindVertexArray(renderer->vao);

    const float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], 0.0f});

    glm_translate(model, (vec3) {0.5f * size[0], 0.5f * size[1], 0.0f});
    glm_rotate(model, glm_rad(rotationX), (vec3) {1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(rotationY), (vec3) {0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(rotationZ), (vec3) {0.0f, 0.0f, 1.0f});
    glm_translate(model, (vec3) {-0.5f * size[0], -0.5f * size[1], 0.0f});

    glm_scale(model, (vec3) {size[0], size[1], 1.0f});

    compoundShaderUse(renderer->shader);
    compoundShaderSetMat4(renderer->shader, "model", model);
    compoundShaderSetVec4(renderer->shader, "color", color);

    glPolygonMode(GL_FRONT_AND_BACK, fill ? GL_FILL : GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void shapeRendererDrawRectangle(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    float rotationX,
    float rotationY,
    float rotationZ,
    const vec4 color,
    bool fill
) {
    glBindVertexArray(renderer->vao);

    const float vertices[] = {
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f, 0.5f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    const unsigned indicesFilled[] = {
        0, 1, 3,
        1, 2, 3
    };

    const unsigned indicesUnfilled[] = {
        0, 1, 2, 3,
        1, 2, 0, 3
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        fill ? sizeof(indicesFilled) : sizeof(indicesUnfilled),
        fill ? indicesFilled : indicesUnfilled,
        GL_DYNAMIC_DRAW
    );

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], 0.0f});

    glm_translate(model, (vec3) {0.5f * size[0], 0.5f * size[1], 0.0f});
    glm_rotate(model, glm_rad(rotationX), (vec3) {1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(rotationY), (vec3) {0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(rotationZ), (vec3) {0.0f, 0.0f, 1.0f});
    glm_translate(model, (vec3) {-0.5f * size[0], -0.5f * size[1], 0.0f});

    glm_scale(model, (vec3) {size[0], size[1], 1.0f});

    compoundShaderUse(renderer->shader);
    compoundShaderSetMat4(renderer->shader, "model", model);
    compoundShaderSetVec4(renderer->shader, "color", color);

    glDrawElements(fill ? GL_TRIANGLES : GL_LINES, fill ? 6 : 8, GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void shapeRendererDrawHorizontalLine(
    const ShapeRenderer* renderer,
    const vec2 position,
    const vec2 size,
    const vec4 color
) {
    glBindVertexArray(renderer->vao);

    const float vertices[] = {
        -0.5f, 0.0f,
        0.5f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], 0.0f});
    glm_scale(model, (vec3) {size[0], size[1], 1.0f});

    compoundShaderUse(renderer->shader);
    compoundShaderSetMat4(renderer->shader, "model", model);
    compoundShaderSetVec4(renderer->shader, "color", color);

    glDrawArrays(GL_LINES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
