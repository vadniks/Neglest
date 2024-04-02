#version 330 core

in vec3 color;
in vec3 position;
in vec2 textureCoords;

out vec4 colorOut;

uniform sampler2D texture1In;
uniform sampler2D texture2In;
uniform float mixIn;

void main() {
    colorOut = mix(texture(texture1In, textureCoords), texture(texture2In, vec2(textureCoords.x, -textureCoords.y)), mixIn);
}