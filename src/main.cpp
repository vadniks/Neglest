
#include "ResourceManager.hpp"
#include "Game.hpp"
#include <cassert>
#include <SDL2/SDL.h>
#include <GL/glew.h>

static const int WIDTH = 1200, HEIGHT = 675;
static Game gGame(WIDTH, HEIGHT);

static void loop(SDL_Window* window) {
    SDL_Event event;

    while (true) {
        SDL_Keycode keyCode;
        bool keyPressed = false;

        while (SDL_PollEvent(&event) == 1) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    keyCode = event.key.keysym.sym;
                    keyPressed = true;
                    break;
                case SDL_KEYUP:
                    keyPressed = false;
                    break;
            }
        }

        gGame.processInput(keyPressed ? &keyCode : nullptr);
        gGame.update();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gGame.render();

        SDL_GL_SwapWindow(window);
    }
}

int main() {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0);
    auto resourceManager = ResourceManager::instance();

    SDL_version version;
    SDL_GetVersion(&version);
    assert(version.major == 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    assert(window != nullptr);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    assert(glewInit() == GLEW_OK);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    SDL_GL_SetSwapInterval(1);
    glViewport(0, 0, WIDTH, HEIGHT);

    gGame.init();

    loop(window);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    delete resourceManager;
    SDL_Quit();

    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
