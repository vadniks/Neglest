#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D sprite;
uniform vec4 spriteColor;

void main() {
    color = spriteColor * texture(sprite, textureCoords);
}