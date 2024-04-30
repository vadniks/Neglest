
#include "compoundShader.h"
#include "spriteRenderer.h"
#include "texture.h"
#include "shapeRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <cglm/cam.h>

static int gWidth = 0, gHeight = 0;
static TTF_Font* gFont = nullptr;

static void renderFrame() {
    mat4 projection;
    glm_ortho(0.0f, (float) gWidth, 0.0f, (float) gHeight, -1.0f, 1.0f, projection);

    // sprites

    /*CompoundShader* spriteShader = compoundShaderCreate("shaders/spriteVertex.glsl", "shaders/spriteFragment.glsl");
    compoundShaderUse(spriteShader);
    compoundShaderSetMat4(spriteShader, "projection", projection);
    compoundShaderSetInt(spriteShader, "sprite", 0);

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(gFont, "Hello OpenGL!", (SDL_Color) {255, 255, 255, 255});
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(textSurface);

    Texture* texture = textureCreate(surface->w, surface->h, surface->pixels);
    int surfaceWidth = surface->w, surfaceHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Surface* surface2 = IMG_Load("res/awesomeface.png");
    Texture* texture2 = textureCreate(surface2->w, surface2->h, surface2->pixels);
    SDL_FreeSurface(surface2);

    SpriteRenderer* spriteRenderer = spriteRendererCreate(spriteShader);
    spriteRendererDraw(
        spriteRenderer,
        texture,
        (vec2) {200.0f, 200.0f},
        (vec2) {(float) surfaceWidth, (float) surfaceHeight},
        180.0f,
        0.0f,
        0.0f,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );

    spriteRendererDraw(
        spriteRenderer,
        texture2,
        (vec2) {550.0f, 200.0f},
        (vec2) {300.0f, 300.0f},
        180.0f,
        0.0f,
        0.0f,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );

    textureDestroy(texture);
    textureDestroy(texture2);
    spriteRendererDestroy(spriteRenderer);
    compoundShaderDestroy(spriteShader);*/

    // shapes

    CompoundShader* shapeShader = compoundShaderCreate("shaders/shapeVertex.glsl", "shaders/shapeFragment.glsl");
    compoundShaderUse(shapeShader);
    compoundShaderSetMat4(shapeShader, "projection", projection);

    ShapeRenderer* shapeRenderer = shapeRendererCreate(shapeShader);

    shapeRendererDrawTriangle(
        shapeRenderer,
        (vec2) {100.0f, 100.0f},
        (vec2) {100.0f, 100.0f},
        0.0f,
        0.0f,
        0.0f,
        (vec4) {1.0f, 1.0f, 1.0f, 1.0f},
        true
    );

    shapeRendererDestroy(shapeRenderer);
    compoundShaderDestroy(shapeShader);
}

static void renderLoop(SDL_Window* window) {
    SDL_Event event;

    while (true) {
        SDL_GL_GetDrawableSize(window, &gWidth, &gHeight);
        glViewport(0, 0, gWidth, gHeight);

        while (SDL_PollEvent(&event) == 1) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderFrame();

        SDL_GL_SwapWindow(window);
    }
}

int main() {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0);
    assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) > 0);
    assert(TTF_Init() == 0); {
        gFont = TTF_OpenFont("res/Roboto-Regular.ttf", 50);

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
            1200,
            675,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
        assert(window != nullptr); {

            SDL_GLContext glContext = SDL_GL_CreateContext(window); {
                glewExperimental = GL_TRUE;
                assert(glewInit() == GLEW_OK);

                glEnable(GL_MULTISAMPLE);
                glEnable(GL_BLEND);

                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

                SDL_GL_SetSwapInterval(1);

                renderLoop(window);
            } SDL_GL_DeleteContext(glContext);
        } SDL_DestroyWindow(window);

        TTF_CloseFont(gFont);
    } TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    assert(SDL_GetNumAllocations() == 0);
    return 0;
}
