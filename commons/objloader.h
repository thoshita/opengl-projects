#ifndef OPENGL_2_OBJLOADER_H
#define OPENGL_2_OBJLOADER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <cstring>

bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
);

#endif