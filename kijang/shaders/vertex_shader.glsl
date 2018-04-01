#version 330 core // same as openGL version

// Input vertex data, different for all executions of this shader.
in vec3 position;
in vec3 in_color;

out vec3 out_color;

uniform mat4 MVP;

void main() {
    out_color = position + vec3(0.3, 0.3, 0.3);
    gl_Position = MVP * vec4(position, 1.0);
}