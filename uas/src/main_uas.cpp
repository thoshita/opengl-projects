#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <commons/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <commons/texture.h>

using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SIDES_OF_CIRCLE 100
#define NUM_OF_WHEELS 5
#define VERTEX_SHADER_FILE_PATH  "uas/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "uas/shaders/fragment_shader.glsl"
#define BITMAP_PATH "uas/uvtemplate.bmp"

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

    GLuint Texture = loadBMP_custom(BITMAP_PATH);
    GLuint TextureID  = glGetUniformLocation(shader_program, "myTexture");

    glm::mat4 Projection = glm::perspective(glm::radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View;
    glm::mat4 Model      = glm::mat4(1.0f);
    glm::mat4 MVP;

    vector<int> start_of_section;
    GLfloat _vertices[][6] = {
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

    GLfloat vertices[64][3];
    GLfloat uv_vertices[64][2];

    for (int i = 0; i < 64; i++) {
        vertices[i][0] = _vertices[i][0];
        vertices[i][1] = _vertices[i][1];
        vertices[i][2] = _vertices[i][2];
        uv_vertices[i][0] = (vertices[i][0] + 1) / 2;
        uv_vertices[i][1] = (vertices[i][1] + 1) / 2;
    }

    GLuint vertex_buffer;

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint uv_buffer;

    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vertices), uv_vertices, GL_STATIC_DRAW);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glBindVertexArray(vertex_array);
    glUseProgram(shader_program);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLfloat camera_x = 0, camera_y = 0, camera_z = -3;
    GLfloat direction_x = 0, direction_y = 0, direction_z = 3;
    GLfloat acceleration = 0.1;

    do {
        View = glm::lookAt(
                glm::vec3(camera_x,camera_y,camera_z),
                glm::vec3(camera_x + direction_x, camera_y + direction_y, camera_z + direction_z),
                glm::vec3(0,1,0)
        );
        MVP = Projection * View * Model;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(shader_program);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size : U+V => 2
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
        glDrawArrays(GL_TRIANGLE_FAN, 5, 7);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 5);
        glDrawArrays(GL_TRIANGLE_FAN, 17, 7);

        for(int i = 0; i < 10; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, 24 + i*4, 4);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
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

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            camera_z -= acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            camera_z += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            direction_y += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            direction_y -= acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            direction_x += acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            direction_x -= acceleration;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            direction_x = -camera_x;
            direction_y = -camera_y;
            direction_z = -camera_z;
        }

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}