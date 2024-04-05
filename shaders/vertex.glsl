#version 330 core

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 textureCoordsIn;

out vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(positionIn, 1.0);
    textureCoords = textureCoordsIn;
}