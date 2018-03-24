cmake_minimum_required(VERSION 3.9)
project(opengl_2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(TRIANGLE_HEADERS
        commons/shader.h
        )

set(TRIANGLE_SOURCES
        triangle/src/main.cpp
        commons/shader.cpp)

add_executable(triangle
        ${TRIANGLE_HEADERS}
        ${TRIANGLE_SOURCES}
        )

target_link_libraries(triangle
        ${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
        glfw
        ${GLEW_LIBRARY}
        )