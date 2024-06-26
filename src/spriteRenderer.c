/*
 * Neglest - an OpenGL 2D game.
 * Copyright (C) 2024  Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    float rotationX,
    float rotationY,
    float rotationZ,
    const vec4 color
) {
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
    compoundShaderSetVec4(renderer->shader, "spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    textureBind(texture);

    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
