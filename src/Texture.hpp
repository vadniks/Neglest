
#pragma once

#include "defs.hpp"
#include <GL/glew.h>

class Texture {
private:
    unsigned mId;
    int mWidth, mHeight;
    int mInternalFormat;
    unsigned mImageFormat;
    int mWrapS, mWrapT;
    int mFilterMin, mFilterMax;
public:
    Texture(unsigned width, unsigned height, byte* data);
    ~Texture();
    void bind() const;
};
