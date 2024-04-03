#version 330 core

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec2 textureCoordsIn;

out vec3 color;
out vec3 position;
out vec2 textureCoords;

uniform mat4 transformIn;

void main() {
    position = positionIn;
    gl_Position = transformIn * vec4(position, 1.0);
    color = colorIn;
    textureCoords = textureCoordsIn;
}