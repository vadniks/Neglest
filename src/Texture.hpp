
#pragma once

#include "defs.hpp"
#include <GL/glew.h>

class Texture {
private:
    unsigned mId;
    int mWidth, mHeight;
public:
    Texture(int width, int height, bool alpha, byte* data);
    Texture(const Texture&) = delete;
    Texture& operator =(const Texture&) = delete;
    ~Texture();
    void bind() const;
};
