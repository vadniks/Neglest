
#pragma once

#include "GameObject.hpp"
#include "Shader.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float life;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

class ParticleGenerator {
private:
    const int particlesCount = 500;
    std::vector<Particle> mParticles;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<Texture> mTexture;
    unsigned mVao;
    unsigned mVbo;
public:
    ParticleGenerator(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture);
    ~ParticleGenerator();
    ParticleGenerator(const ParticleGenerator&) = delete;
    ParticleGenerator& operator =(const ParticleGenerator&) = delete;
    void update(GameObject* object, glm::vec2 offset);
    void draw();
private:
    int firstUnusedParticle();
    void respawnParticle(Particle& particle, GameObject* object, glm::vec2 offset);
};
