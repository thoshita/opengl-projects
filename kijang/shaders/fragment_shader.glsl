#version 330 core // same as openGL version

in vec3 out_color; // variable name must be the same as output from vertex shader
out vec4 frag_color; // (r, g, b, a)

void main() {
  frag_color = vec4(out_color, 1.0);
}