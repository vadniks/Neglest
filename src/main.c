
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <glad/glad.h> // https://glad.dav1d.de/

#ifdef __clang__
#   define nullable _Nullable
#   define nonnull _Nonnull
#else
#   define nullable
#   define nonnull
#endif

static void assert(bool condition) { if (!condition) abort(); }

static void renderLoop(SDL_Window* nonnull window, SDL_GLContext* nonnull glContext) {
    SDL_Event event;
    int width, height;

    while (true) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT)
                return;

            if (event.type == SDL_WINDOWEVENT)
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    glViewport(0, 0, event.window.data1, event.window.data1);
        }

        SDL_GL_SwapWindow(window);
    }
}

int main(void) {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0); {

        SDL_version version;
        SDL_GetVersion(&version);
        assert(version.major == 2);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        SDL_Window* window = SDL_CreateWindow(
            "title",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            512,
            512,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
        assert(window != nullptr); {
            SDL_GLContext* glContext = SDL_GL_CreateContext(window); {
                assert(gladLoadGLLoader((GLADloadproc) &SDL_GL_GetProcAddress) == 1);
                SDL_GL_SetSwapInterval(1);

                renderLoop(window, glContext);

            } SDL_GL_DeleteContext(glContext);
        } SDL_DestroyWindow(window);

    } SDL_Quit();
    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
