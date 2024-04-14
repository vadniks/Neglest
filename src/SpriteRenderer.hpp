
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
    static inline const float DEFAULT_SIZE = 10.0f;

    explicit SpriteRenderer(const std::shared_ptr<Shader>& shader);
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator =(const SpriteRenderer&) = delete;
    ~SpriteRenderer();

    void draw(
        const std::shared_ptr<Texture>& texture,
        glm::vec2 position,
        glm::vec2 size = glm::vec2(DEFAULT_SIZE, DEFAULT_SIZE),
        float rotation = 0.0f,
        glm::vec3 color = glm::vec3(1.0f)
    );
};
