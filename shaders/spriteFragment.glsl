#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main() {
    color = spriteColor * texture(image, texCoords);
}