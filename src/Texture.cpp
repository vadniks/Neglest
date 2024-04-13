
#include "Texture.hpp"

Texture::Texture(unsigned width, unsigned height, byte* data) {
    mWidth = width;
    mHeight = height;
    mInternalFormat = GL_RGB;
    mImageFormat = GL_RGB;
    mWrapS = GL_REPEAT;
    mWrapT = GL_REPEAT;
    mFilterMin = GL_LINEAR;
    mFilterMax = GL_LINEAR;

    glGenTextures(1, (unsigned[1]) {mId});
    glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mImageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, (unsigned[1]) {mId});
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, mId);
}
