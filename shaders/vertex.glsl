#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 colorIn;
out vec3 color;
out vec3 position;
uniform float offset;

void main() {
    position = vec3(positionIn.x + offset, -positionIn.y, positionIn.z);
    gl_Position = vec4(position, 1.0);
    color = colorIn;
}