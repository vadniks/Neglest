#version 330 core

in vec2 textureCoords;

out vec4 colorOut;

uniform sampler2D texture1In;
uniform sampler2D texture2In;

void main() {
    colorOut = mix(texture(texture1In, textureCoords), texture(texture2In, vec2(textureCoords.x, -textureCoords.y)), 0.2);
}