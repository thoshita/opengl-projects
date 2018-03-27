#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <commons/shader.h>
#include <commons/drawer.h>

using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SIDES_OF_CIRCLE 100
#define NUM_OF_WHEELS 5
#define VERTEX_SHADER_FILE_PATH  "car/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "car/shaders/fragment_shader.glsl"

void errorCallback(int error, const char * description) {
    cout << "Error " << error << ": " << description << endl;
}

void drawWheels(Vertices &vertices, GLint &num_of_points, vector<int> &start_of_section) {
    for(int i = 0; i < NUM_OF_WHEELS; i++) {
        combineVertices(vertices,
                        num_of_points,
                        createCircle(-0.45f, -0.25f, 1, 1, 1, 0.15, SIDES_OF_CIRCLE, SIDES_OF_CIRCLE*i/NUM_OF_WHEELS),
                        SIDES_OF_CIRCLE + 2,
                        start_of_section);

        combineVertices(vertices,
                        num_of_points,
                        createCircle(0.45f, -0.25f, 1, 1, 1, 0.15, SIDES_OF_CIRCLE,
                                     SIDES_OF_CIRCLE*(NUM_OF_WHEELS/2+i)/NUM_OF_WHEELS),
                        SIDES_OF_CIRCLE + 2,
                        start_of_section);
    }
}

void drawAtap(Vertices &vertices, GLint &num_of_points, vector<int> &start_of_section) {
    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            -0.15f, 0.15f,
                            0.85f, 0.15f,
                            0.15f, 0.45f,

                            1, 0, 0),
                    3,
                    start_of_section);
}

void drawKaca(Vertices &vertices, GLint &num_of_points, vector<int> &start_of_section) {
    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            -0.15f, 0.15f,
                            0.10f, 0.15f,
                            0.10f, 0.35f,

                            1, 1, 1),
                    3,
                    start_of_section);

    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            0.10f, 0.15f,
                            0.10f, 0.35f,
                            0.13f, 0.40f,

                            1, 1, 1),
                    3,
                    start_of_section);

    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            0.10f, 0.15f,
                            0.13f, 0.40f,
                            0.25f, 0.35f,

                            1, 1, 1),
                    3,
                    start_of_section);
}

void drawPintu(Vertices &vertices, GLint &num_of_points, vector<int> &start_of_section) {
    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            -0.15f, -0.30f,
                            -0.15f, 0.10f,
                            0.30f, 0.15f,

                            0.5f, 0, 0),
                    3,
                    start_of_section);

    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            0.30f, 0.15f,
                            0.15f, -0.30f,
                            0.15f, -0.20f,

                            0.5f, 0, 0),
                    3,
                    start_of_section);

    combineVertices(vertices,
                    num_of_points,
                    createTriangle(
                            0.15f, -0.20f,
                            0.15f, -0.30f,
                            -0.15f, -0.30f,

                            0.5f, 0, 0),
                    3,
                    start_of_section);
}

void drawBadan(Vertices &vertices, GLint &num_of_points, vector<int> &start_of_section) {
    GLfloat points[][6] = {
            {
                    -0.30f, -0.35f,
                    0.30f, -0.35f,
                    0.30f, 0.15f
            },
            {
                    -0.30f, -0.35f,
                    -0.30f, 0.15f,
                    0.30f, 0.15f
            },
            {
                    0.30f, -0.35f,
                    0.30f, 0.15f,
                    0.85f, 0.15f
            },
            {
                    0.30f, -0.35f,
                    0.85f, -0.25f,
                    0.85f, 0.15f
            },
            {
                    -0.30f, -0.35f,
                    -0.30f, 0.15f,
                    -0.60f, 0.15f
            },
            {
                    -0.30f, -0.35f,
                    -0.60f, -0.35f,
                    -0.70f, 0.f
            },
            {
                    -0.60f, 0.f,
                    -0.60f, 0.15f,
                    -0.75f, 0.f
            },
            {
                    -0.75f, 0.f,
                    -0.60f, 0.f,
                    -0.70f, -0.20f
            },
            {
                    -0.70f, -0.20f,
                    -0.60f, 0.f,
                    -0.60f, -0.35f
            }
    };

    GLfloat r = 0.95f, g = 0, b = 0;

    for(int i = 0; i < 8; i++) {
        combineVertices(vertices, num_of_points, createTriangle(
            points[i][0], points[i][1], points[i][2], points[i][3], points[i][4], points[i][5], r, g, b
        ), 3, start_of_section);
    }
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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Car", nullptr, nullptr);
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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint shader_program = LoadShaders(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);

    vector<int> start_of_section;
    auto vertices = new GLfloat;
    int num_of_points = 0;

    drawWheels(vertices, num_of_points, start_of_section);
    drawBadan(vertices, num_of_points, start_of_section);
    drawAtap(vertices, num_of_points, start_of_section);
    drawKaca(vertices, num_of_points, start_of_section);
    drawPintu(vertices, num_of_points, start_of_section);

    GLuint vertex_buffer;

    glGenBuffers(1, &vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * num_of_points, vertices, GL_STATIC_DRAW);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(static_cast<GLuint>(posAttrib));
    glVertexAttribPointer(static_cast<GLuint>(posAttrib), 2, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), nullptr);

    auto colAttrib = glGetAttribLocation(shader_program, "in_color");
    glEnableVertexAttribArray(static_cast<GLuint>(colAttrib));
    glVertexAttribPointer(static_cast<GLuint>(colAttrib), 3, GL_FLOAT, GL_FALSE,
                          5*sizeof(float), (void*)(2*sizeof(float)));

    glBindVertexArray(0);
    glUseProgram(shader_program);

    GLint i = 0;
    GLint counter = 0;

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertex_array);

        for(int i = NUM_OF_WHEELS*2; i < start_of_section.size(); i++) {
            glDrawArrays(GL_TRIANGLES, start_of_section[i], 3);
        };

        glDrawArrays(GL_TRIANGLE_FAN, start_of_section[i*2], SIDES_OF_CIRCLE + 2);
        glDrawArrays(GL_TRIANGLE_FAN, start_of_section[i*2 + 1], SIDES_OF_CIRCLE + 2);


        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (++counter == 5) {
            counter = 0;
            i++;
            i %= NUM_OF_WHEELS;
        }

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}