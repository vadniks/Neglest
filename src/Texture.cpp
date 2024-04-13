
#include "Texture.hpp"
#include <cassert>

Texture::Texture(int width, int height, byte* data) {
    mId = 0;
    assert(width >= 0 && height >= 0);
    mWidth = width;
    mHeight = height;

    glGenTextures(1, (unsigned[1]) {mId});
    glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, (unsigned[1]) {mId});
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, mId);
}
