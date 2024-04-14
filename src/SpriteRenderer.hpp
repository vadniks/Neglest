
#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include <memory>
#include <glm/glm.hpp>

class SpriteRenderer {
private:
    std::shared_ptr<Shader> mShader;
    unsigned mVao, mVbo;
public:
    static inline const float defaultSize = 10.0f;

    SpriteRenderer(std::shared_ptr<Shader> shader);
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator =(const SpriteRenderer&) = delete;
    ~SpriteRenderer();

    void draw(
        std::shared_ptr<Texture> texture,
        glm::vec2 position,
        glm::vec2 size = glm::vec2(defaultSize, defaultSize),
        float rotation = 0.0f,
        glm::vec3 color = glm::vec3(1.0f)
    );
};
