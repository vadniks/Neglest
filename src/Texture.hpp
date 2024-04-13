
#pragma once

#include "defs.hpp"
#include <GL/glew.h>

class Texture {
private:
    unsigned mId;
    int mWidth, mHeight;
public:
    Texture(int width, int height, byte* data);
    ~Texture();
    void bind() const;
};
