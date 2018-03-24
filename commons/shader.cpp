#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Load vertex and fragment shader and return the shader program
GLuint LoadShaders(const char * vertex_shader_file_path, const char * fragment_shader_file_path) {
    GLint success = GL_FALSE;
    char info_log[512];

    // Read the vertex shader source code from the file
    std::string vertex_shader_source;
    std::ifstream vertex_shader_stream(vertex_shader_file_path, std::ios::in);
    vertex_shader_stream.open(vertex_shader_file_path);
    if (vertex_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << vertex_shader_stream.rdbuf();
        vertex_shader_source = sstr.str();
        vertex_shader_stream.close();
    } else {
        std::cout << "Failed to open " << vertex_shader_file_path << "." << std::endl;
        return 0;
    }

    // Compile vertex shader
    std::cout << "Compiling shader: " << vertex_shader_file_path << std::endl;
    char const * vertex_source_pointer = vertex_shader_source.c_str();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source_pointer, NULL);
    glCompileShader(vertex_shader);

    // Check vertex shader
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << info_log << std::endl;
    }


    // Read the fragment shader source code from the file
    std::string fragment_shader_source;
    std::ifstream fragment_shader_stream(fragment_shader_file_path, std::ios::in);
    if (fragment_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << fragment_shader_stream.rdbuf();
        fragment_shader_source = sstr.str();
        fragment_shader_stream.close();
    } else {
        std::cout << "Failed to open " << vertex_shader_file_path << "." << std::endl;
        return 0;
    }

    // Compile fragment shader
    std::cout << "Compiling shader: " << fragment_shader_file_path << std::endl;
    char const * fragment_source_pointer = fragment_shader_source.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source_pointer, NULL);
    glCompileShader(fragment_shader);

    // Check fragment shader
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << info_log << std::endl;
    }

    // Link the program
    std::cout << "Linking program..." << std::endl;
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Check the program
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::LINK_FAILED" << std::endl << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}