#version 450 core

layout(location = 0) in vec2 a_position;

out vec2 position;

void main() {
    gl_Position = vec4(a_position, 0.0, 1.0);
    position = a_position;
}