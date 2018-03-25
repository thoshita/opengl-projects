#version 330 core // same as openGL version

// Input vertex data, different for all executions of this shader.
in vec2 position;
in vec3 in_color;

out vec3 out_color;

void main() {
    out_color = in_color;
    gl_Position = vec4(position, 0.0, 1.0);
}