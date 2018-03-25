#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <commons/shader.h>
using namespace std;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define VERTEX_SHADER_FILE_PATH  "triangle/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "triangle/shaders/fragment_shader.glsl"

void error_callback(int error, const char * description) {
    cout << description << endl;
}

int main() {
    glfwSetErrorCallback(error_callback);
    // Initialize GLFW
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create and compile GLSL program from the shaders
    GLuint shader_program = LoadShaders(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);

    // Create triangle points and set their colors
    static const GLfloat triangle_points[] = {
            -1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
            1.0f, -1.0f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
    };

    // Create VBO
    GLuint vertex_buffer;

    // Generate 1 buffer, put the resulting identifier in vertex_buffer
    glGenBuffers(1, &vertex_buffer);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_points), triangle_points, GL_STATIC_DRAW);

    // Create VAO
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    // Bind VAO
    glBindVertexArray(vertex_array);

    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shader_program, "in_color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), (void*)(2*sizeof(float)));

    // unbind VAO
    glBindVertexArray(0);

    // Main loop
    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader program
        glUseProgram(shader_program);

        // load vertex atrributes from VAO

        glBindVertexArray(vertex_array);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        // unbind VAO
        glBindVertexArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
    } while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup VBO, VAO, and shader program
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(shader_program);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}