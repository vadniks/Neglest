
#include "ResourceManager.hpp"
#include <fstream>
#include <sstream>
#include <SDL2/SDL_image.h>

static ResourceManager* gInstance = nullptr;

ResourceManager::ResourceManager() {
    IMG_Init(IMG_INIT_PNG);
}

ResourceManager::~ResourceManager() {
    IMG_Quit();

    for (auto& i : mShaders) {
        const int uses = static_cast<int>(i.second.use_count());
        for (int j = 0; j < uses; i.second.reset(), j++);
        delete i.second.get();
    }
    mShaders.clear();

    for (auto& i : mTextures) {
        const int uses = static_cast<int>(i.second.use_count());
        for (int j = 0; j < uses; i.second.reset(), j++);
        delete i.second.get();
    }
    mTextures.clear();

    gInstance = nullptr;
}

ResourceManager* ResourceManager::instance() {
    if (gInstance != nullptr) return gInstance;
    gInstance = new ResourceManager();
    return gInstance;
}

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name) {
    std::ifstream vertexStream(vertexFile), fragmentStream(fragmentFile);
    std::stringstream vertexStrings, fragmentStrings;

    vertexStrings << vertexStream.rdbuf();
    fragmentStrings << fragmentStream.rdbuf();

    vertexStream.close();
    fragmentStream.close();

    std::shared_ptr<Shader> shader(new Shader(vertexStrings.str(), fragmentStrings.str()));
    mShaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name) {
    return mShaders.at(name);
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string& file, bool alpha, const std::string& name) {
    SDL_Surface* surface = IMG_Load(file.c_str());
    std::shared_ptr<Texture> texture(new Texture(
        surface->w,
        surface->h,
        alpha,
        reinterpret_cast<byte*>(surface->pixels)
    ));
    SDL_FreeSurface(surface);
    mTextures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name) {
    return mTextures.at(name);
}
