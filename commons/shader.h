#ifndef OPENGL_2_SHADER_H
#define OPENGL_2_SHADER_H

#include <GL/glew.h>

// Load vertex and fragment shader and return the shader program id
GLuint LoadShaders(const char * vertex_shader_file_path, const char * fragment_shader_file_path);

#endif //OPENGL_2_SHADER_H
