/*
 * Neglest - An OpenGL 3D game.
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
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

#include "defs.hpp"
#include "Shader.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h> // https://glad.dav1d.de/
//#include <glm/glm.hpp>

static void renderFrame() {
    float vertices[] = {
        // positions        // colors        // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };
    unsigned indices[] = {
        0, 1, 2, 3
    };

    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    unsigned ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_Surface* surface = IMG_Load("images/wall.jpg");
    assert(surface != nullptr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader.use();
    shader.setValue("offset", 0.5f);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    glBindTexture(1, 0);
    glDeleteTextures(1, &texture);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

static void renderLoop(SDL_Window* window) {
    SDL_Event event;
    int width, height;

    while (true) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT)
                return;
        }

        SDL_GL_GetDrawableSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderFrame();

        SDL_GL_SwapWindow(window);
    }
}

int main() {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0);
    assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) > 0); {

        SDL_version version;
        SDL_GetVersion(&version);
        assert(version.major == 2);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        const int scale = 50;
        SDL_Window* window = SDL_CreateWindow(
            "Title",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            16 * scale,
            9 * scale,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
        assert(window != nullptr); {

            SDL_GLContext glContext = SDL_GL_CreateContext(window); {
                assert(gladLoadGLLoader((GLADloadproc) &SDL_GL_GetProcAddress) == 1);
                SDL_GL_SetSwapInterval(1);

                renderLoop(window);

            } SDL_GL_DeleteContext(glContext);

        } SDL_DestroyWindow(window);

    } IMG_Quit();
    SDL_Quit();
    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
