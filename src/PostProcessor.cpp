
#include "PostProcessor.hpp"
#include <GL/glew.h>

PostProcessor::PostProcessor(const std::shared_ptr<Shader>& shader, int width, int height) {
    mTex = 0;
    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mFbo = 0;
    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0);


}

PostProcessor::~PostProcessor() {
    glDeleteFramebuffers(1, &mFbo);
}

void PostProcessor::beginRender() {

}

void PostProcessor::render() {

}

void PostProcessor::endRender() {

}
