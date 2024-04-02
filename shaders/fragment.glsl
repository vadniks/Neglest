#version 330 core

in vec3 color;
in vec3 position;
in vec2 textureCoords;

out vec4 colorOut;

uniform sampler2D textureIn;

void main() {
    colorOut = texture(textureIn, textureCoords);
}