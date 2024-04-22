
#include "ParticleGenerator.hpp"

static int gLastUsedParticle = 0;

ParticleGenerator::ParticleGenerator(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture) {
    mShader = shader;
    mTexture = texture;
    mVao = 0;
    mVbo = 0;

    for (int i = 0; i < particlesCount; i++)
        mParticles.emplace_back();

    float quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindVertexArray(0);
}

ParticleGenerator::~ParticleGenerator() {
    glDeleteBuffers(1, &mVbo);
    glDeleteVertexArrays(1, &mVao);
}

void ParticleGenerator::update(GameObject* object, glm::vec2 offset) {
    const int newParticles = 2;

    for (unsigned int i = 0; i < newParticles; i++)
        respawnParticle(mParticles[firstUnusedParticle()], object, offset);

    const float lifePoint = 0.1f;
    for (int i = 0; i < particlesCount; i++) {
        Particle& p = mParticles[i];
        p.life -= lifePoint;

        if (p.life > 0.0f) {
            p.position -= p.velocity * lifePoint;
            p.color.a -= lifePoint * 2.5f;
        }
    }
}

void ParticleGenerator::draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    mShader->use();
    for (Particle particle : mParticles) {
        if (particle.life <= 0.0f) continue;

        mShader->setValue("offset", particle.position);
        mShader->setValue("color", particle.color);

        mTexture->bind();
        glBindVertexArray(mVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

int ParticleGenerator::firstUnusedParticle() {
    for (int i = gLastUsedParticle; i < particlesCount; i++) {
        if (mParticles[i].life <= 0.0f) {
            gLastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < gLastUsedParticle; i++) {
        if (mParticles[i].life <= 0.0f) {
            gLastUsedParticle = i;
            return i;
        }
    }

    gLastUsedParticle = 0;
    return 0;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-msc50-cpp"
void ParticleGenerator::respawnParticle(Particle& particle, GameObject* object, glm::vec2 offset) {
    float random = static_cast<float>((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + (static_cast<float>(rand() % 100) / 100.0f);

    particle.position = object->position() + random + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.velocity = object->velocity() * 0.1f;
}
#pragma clang diagnostic pop
