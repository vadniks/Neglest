#version 330 core

layout (location = 2) in vec2 textureCoordsIn;

out vec2 textureCoords;

void main() {
    gl_Position = vec4(position, 1.0);
    textureCoords = textureCoordsIn;
}