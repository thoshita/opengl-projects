#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <commons/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SIDES_OF_CIRCLE 100
#define NUM_OF_WHEELS 5
#define VERTEX_SHADER_FILE_PATH  "kijang/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "kijang/shaders/fragment_shader.glsl"

void errorCallback(int error, const char * description) {
    cout << "Error " << error << ": " << description << endl;
}

int main() {
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kijang", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = static_cast<GLboolean>(true);
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint shader_program = LoadShaders(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);
    GLuint matrix = glGetUniformLocation(shader_program, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View;
    glm::mat4 Model      = glm::mat4(1.0f);
    glm::mat4 MVP;

    vector<int> start_of_section;
    GLfloat vertices[][6] = {
            {-0.9f, -0.2f, 0.f, 1.f, 0.f, 0.f},
            {-0.9f, 0.f, 0.f, 1.f, 0.f, 0.f},
            {-0.8f, 0.1f, 0.f, 1.f, 0.f, 0.f},
            {-0.5f, 0.2f, 0.f, 1.f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.f, 1.f, 0.f, 0.f},

            {-0.5f, 0.2f, 0.f, 1.f, 0.f, 0.f},
            {-0.1f, 0.4f, 0.f, 1.f, 0.f, 0.f},
            {0.8f, 0.4f, 0.f, 1.f, 0.f, 0.f},
            {0.9f, 0.2f, 0.f, 1.f, 0.f, 0.f},
            {0.9f, -0.1f, 0.f, 1.f, 0.f, 0.f},
            {0.8f, -0.2f, 0.f, 1.f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.f, 1.f, 0.f, 0.f},

            {-0.9f, -0.2f, 0.5f, 1.f, 0.f, 0.f},
            {-0.9f, 0.f, 0.5f, 1.f, 0.f, 0.f},
            {-0.8f, 0.1f, 0.5f, 1.f, 0.f, 0.f},
            {-0.5f, 0.2f, 0.5f, 1.f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.5f, 1.f, 0.f, 0.f},

            {-0.5f, 0.2f, 0.5f, 1.f, 0.f, 0.f},
            {-0.1f, 0.4f, 0.5f, 1.f, 0.f, 0.f},
            {0.8f, 0.4f, 0.5f, 1.f, 0.f, 0.f},
            {0.9f, 0.2f, 0.5f, 1.f, 0.f, 0.f},
            {0.9f, -0.1f, 0.5f, 1.f, 0.f, 0.f},
            {0.8f, -0.2f, 0.5f, 1.f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.5f, 1.f, 0.f, 0.f},

            {-0.9f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.9f, 0.f, 0.f, 0.5f, 0.f, 0.f},
            {-0.9f, 0.f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.9f, -0.2f, 0.5f, 0.5f, 0.f, 0.f},

            {-0.9f, 0.f, 0.f, 0.5f, 0.f, 0.f},
            {-0.8f, 0.1f, 0.f, 0.5f, 0.f, 0.f},
            {-0.8f, 0.1f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.9f, 0.f, 0.5f, 0.5f, 0.f, 0.f},

            {-0.8f, 0.1f, 0.f, 0.5f, 0.f, 0.f},
            {-0.5f, 0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.5f, 0.2f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.8f, 0.1f, 0.5f, 0.5f, 0.f, 0.f},

            {-0.5f, 0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.1f, 0.4f, 0.f, 0.5f, 0.f, 0.f},
            {-0.1f, 0.4f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.5f, 0.2f, 0.5f, 0.5f, 0.f, 0.f},

            {-0.1f, 0.4f, 0.f, 0.5f, 0.f, 0.f},
            {0.8f, 0.4f, 0.f, 0.5f, 0.f, 0.f},
            {0.8f, 0.4f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.1f, 0.4f, 0.5f, 0.5f, 0.f, 0.f},

            {0.8f, 0.4f, 0.f, 0.5f, 0.f, 0.f},
            {0.9f, 0.2f, 0.f, 0.5f, 0.f, 0.f},
            {0.9f, 0.2f, 0.5f, 0.5f, 0.f, 0.f},
            {0.8f, 0.4f, 0.5f, 0.5f, 0.f, 0.f},

            {0.9f, 0.2f, 0.f, 0.5f, 0.f, 0.f},
            {0.9f, -0.1f, 0.f, 0.5f, 0.f, 0.f},
            {0.9f, -0.1f, 0.5f, 0.5f, 0.f, 0.f},
            {0.9f, 0.2f, 0.5f, 0.5f, 0.f, 0.f},

            {0.9f, -0.1f, 0.f, 0.5f, 0.f, 0.f},
            {0.8f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {0.8f, -0.2f, 0.5f, 0.5f, 0.f, 0.f},
            {0.9f, -0.1f, 0.5f, 0.5f, 0.f, 0.f},

            {0.8f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.5f, 0.5f, 0.f, 0.f},
            {0.8f, -0.2f, 0.5f, 0.5f, 0.f, 0.f},

            {-0.5f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.9f, -0.2f, 0.f, 0.5f, 0.f, 0.f},
            {-0.9f, -0.2f, 0.5f, 0.5f, 0.f, 0.f},
            {-0.5f, -0.2f, 0.5f, 0.5f, 0.f, 0.f}
    };

    GLuint vertex_buffer;

    glGenBuffers(1, &vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(static_cast<GLuint>(posAttrib));
    glVertexAttribPointer(static_cast<GLuint>(posAttrib), 3, GL_FLOAT, GL_FALSE,
                          6*sizeof(float), nullptr);

    auto colAttrib = glGetAttribLocation(shader_program, "in_color");
    glEnableVertexAttribArray(static_cast<GLuint>(colAttrib));
    glVertexAttribPointer(static_cast<GLuint>(colAttrib), 3, GL_FLOAT, GL_FALSE,
                          6*sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
    glUseProgram(shader_program);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLfloat camera_x = 0, camera_y = 0, camera_z = -3;
    GLfloat acceleration = 0.5;

    do {
        View = glm::lookAt(
                glm::vec3(camera_x,camera_y,camera_z),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0)
        );
        MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(vertex_array);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
        glDrawArrays(GL_TRIANGLE_FAN, 5, 7);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 5);
        glDrawArrays(GL_TRIANGLE_FAN, 17, 7);

        for(int i = 0; i < 10; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, 24 + i*4, 4);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_y += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_x -= acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_y -= acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_x += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera_z += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera_z -= acceleration;
        }

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}