
#include "defs.hpp"
#include "CompoundShader.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static enum RgbaFormat : unsigned {
    RgbaFormatText = GL_UNSIGNED_INT_8_8_8_8,
    RgbaFormatUsual = GL_UNSIGNED_BYTE
};

static int gWidth = 0, gHeight = 0;
static TTF_Font* gFont = nullptr;

static void drawTexture(int x, int y, const SDL_Surface* surface, RgbaFormat format) {
    unsigned texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        surface->w,
        surface->h,
        0,
        GL_RGBA,
        format,
        surface->pixels
    );

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(gWidth), 0.0f, static_cast<float>(gHeight));

    CompoundShader compoundShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    compoundShader.use();
    compoundShader.setValue("projection", projection);

    const auto xPos = static_cast<float>(x), yPos = static_cast<float>(y), w = static_cast<float>(surface->w), h = static_cast<float>(surface->h);
    float vertices[6][4] = {
        {xPos, yPos + h, 0.0f, 0.0f},
        {xPos, yPos, 0.0f, 1.0f},
        {xPos + w, yPos, 1.0f, 1.0f},
        {xPos, yPos + h, 0.0f, 0.0f},
        {xPos + w, yPos, 1.0f, 1.0f},
        {xPos + w, yPos + h, 1.0f, 0.0f}
    };

    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //

    glDeleteTextures(1, &texture);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
}

static void renderFrame() {
    SDL_Surface* surfaceArgb = TTF_RenderUTF8_Blended(gFont, "Hello World!", (SDL_Color) {0, 0, 0, 0});
    assert(surfaceArgb != nullptr);

    SDL_Surface* surface = SDL_ConvertSurfaceFormat(surfaceArgb, SDL_PIXELFORMAT_RGBA32, 0);
    assert(surface != nullptr);
    SDL_FreeSurface(surfaceArgb);

//    SDL_Surface* surface = IMG_Load("images/aSDL_Surface* surface = IMG_Load("images/awesomeface.png");wesomeface.png");

    drawTexture(0, 0, surface, RgbaFormat::RgbaFormatText);

    SDL_FreeSurface(surface);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderFrame();

        SDL_GL_SwapWindow(window);
    }
}

int main() {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0);
    assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) > 0);
    assert(TTF_Init() == 0); {
        gFont = TTF_OpenFont("font/Roboto-Regular.ttf", 50);

        SDL_version version;
        SDL_GetVersion(&version);
        assert(version.major == 2);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "1");

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
                glEnable(GL_MULTISAMPLE);
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
