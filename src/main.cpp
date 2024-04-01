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

#include <cassert>
#include <SDL2/SDL.h>
#include <glad/glad.h> // https://glad.dav1d.de/
//#include <glm/glm.hpp>

#define USED(x) ((void) x)

static_assert(sizeof(char) == 1 & sizeof(int) == 4 & sizeof(long) == 8 & sizeof(nullptr) == 8);

static void renderFrame() {
    const char* const vertexShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec3 position;

        void main() {
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
    )";

    const unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    assert(success == 1);

    const char* const fragmentShaderCode = R"(
        #version 330 core
        out vec4 finalColor;
        uniform vec4 color;

        void main() {
            finalColor = color;
        }
    )";

    const unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    assert(success == 1);

    const unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    assert(success == 1);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    unsigned indices[] = {
        0, 1, 2
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    const int colorLocation = glGetUniformLocation(shaderProgram, "color");
    assert(colorLocation >= 0);
    glUseProgram(shaderProgram);
    glUniform4f(colorLocation, 0.0f, sin((float) SDL_GetTicks64()) / 2.0f + 0.5f, 0.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glDeleteProgram(shaderProgram);

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
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0); {

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

    } SDL_Quit();
    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
