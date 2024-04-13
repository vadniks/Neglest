
#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include <map>
#include <string>
#include <memory>

class ResourceManager {
private:
    std::map<std::string, std::shared_ptr<Shader>> mShaders;
    std::map<std::string, std::shared_ptr<Texture>> mTextures;

    ResourceManager();
public:
    ~ResourceManager();
    static ResourceManager* instance();
    std::shared_ptr<Shader> loadShader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name);
    std::shared_ptr<Shader> getShader(const std::string& name);
    std::shared_ptr<Texture> loadTexture(const std::string& file, bool alpha, const std::string& name);
    std::shared_ptr<Texture> getTexture(const std::string& name);
};
