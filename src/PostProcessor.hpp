
#pragma once

#include "Shader.hpp"
#include <memory>

class PostProcessor {
private:
    unsigned mTex;
    unsigned mFbo;
    std::shared_ptr<Shader> mShader;
public:
    PostProcessor(const std::shared_ptr<Shader>& shader, int width, int height);
    PostProcessor(const PostProcessor&) = delete;
    ~PostProcessor();
    PostProcessor& operator =(const PostProcessor&) = delete;
    void beginRender();
    void render();
    void endRender();
};
