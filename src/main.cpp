
#include "defs.hpp"
#include "Shader.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void renderFrame(
    float width,
    float height,
    float zoom,
    const glm::vec3& cameraPos,
    const glm::vec3& cameraFront,
    const glm::vec3& cameraUp
) {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_Surface* surface1 = IMG_Load("images/wall.jpg");
    assert(surface1 != nullptr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface1->w, surface1->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface1->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface1);

    unsigned texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_Surface* surface2 = IMG_Load("images/awesomeface.png");
    assert(surface2 != nullptr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface2->w, surface2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface2->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface2);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    auto view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::perspective(glm::radians(zoom), width / height, 0.1f, 100.0f);

    static Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader.use();

    shader.setValue("texture1In", 0);
    shader.setValue("texture2In", 1);
    shader.setValue("view", view);
    shader.setValue("projection", projection);

    for (int i = 0; i < 10; i++) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(i % 3 == 0 ? static_cast<float>(SDL_GetTicks()) / 100.0f : 20.0f * static_cast<float>(i)), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setValue("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindTexture(1, 0);
    glDeleteTextures(1, &texture2);

    glBindTexture(1, 0);
    glDeleteTextures(1, &texture1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

static void processKeyboardPress(
    SDL_Keycode keycode,
    glm::vec3& cameraPos,
    glm::vec3& cameraFront,
    glm::vec3& cameraUp,
    float cameraSpeed
) {
    switch (keycode) {
        case SDLK_w:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case SDLK_a:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
        case SDLK_s:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case SDLK_d:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
        case SDLK_q:
            SDL_Event event;
            event.type = SDL_QUIT;
            SDL_PushEvent(&event);
            break;
        default:
            break;
    }
}

static void processMouseMotion(
    int& lastX,
    int& lastY,
    int xPos,
    int yPos,
    bool firstMouse,
    float& yaw,
    float& pitch,
    glm::vec3& cameraFront
) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
    }

    auto xOffset = static_cast<float>(xPos - lastX);
    auto yOffset = static_cast<float>(lastY - yPos);

    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void renderLoop(SDL_Window* window) {
    SDL_Event event;
    int width, height;
    float deltaTime, lastFrame = 0.0f;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    int lastX = -1, lastY = -1;
    float yaw = 0.0f, pitch = 0.0f, zoom = 45.0f;
    bool firstMouse = true;

    while (true) {
        auto currentFrame = static_cast<float>(SDL_GetTicks());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float cameraSpeed = 0.01f * deltaTime;

        SDL_GL_GetDrawableSize(window, &width, &height);
        glViewport(0, 0, width, height);

        while (SDL_PollEvent(&event) == 1) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    processKeyboardPress(event.key.keysym.sym, cameraPos, cameraFront, cameraUp, cameraSpeed);
                    break;
                case SDL_MOUSEMOTION:
                    processMouseMotion(
                        lastX,
                        lastY,
                        event.motion.x,
                        event.motion.y,
                        firstMouse,
                        yaw,
                        pitch,
                        cameraFront
                    );
                    firstMouse = false;
                    break;
                case SDL_MOUSEWHEEL:
                    zoom -= (float) static_cast<float>(lastY - event.motion.yrel) / 10.0f;
                    if (zoom < 1.0f)
                        zoom = 1.0f;
                    if (zoom > 45.0f)
                        zoom = 45.0f;
                    break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderFrame(static_cast<float>(width), static_cast<float>(height), zoom, cameraPos, cameraFront, cameraUp);

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

        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "1");
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "0");

        const int scale = 50;
        SDL_Window* window = SDL_CreateWindow(
            "OpenGL",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            16 * scale,
            9 * scale,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
        assert(window != nullptr); {

            SDL_GLContext glContext = SDL_GL_CreateContext(window); {
                glewExperimental = GL_TRUE;
                assert(glewInit() == GLEW_OK);

                glEnable(GL_DEPTH_TEST);
                SDL_GL_SetSwapInterval(1);

                renderLoop(window);
            } SDL_GL_DeleteContext(glContext);
        } SDL_DestroyWindow(window);

    } IMG_Quit();
    SDL_Quit();

    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
