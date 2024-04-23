#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D textureIn;

void main() {
    color = texture(textureIn, textureCoords);
}