// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <commons/shader.h>
#include <commons/texture.h>

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "texture/shaders/vertex_shader.glsl", "texture/shaders/fragment_shader.glsl" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Load the texture using any two methods
    GLuint Texture = loadBMP_custom("texture/uvtemplate.bmp");
    //GLuint Texture = loadDDS("uvtemplate.DDS");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTexture");

    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
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

    GLfloat g_vertex_buffer_data[64][3];
    GLfloat g_uv_buffer_data[64][2];

    for (int i = 0; i < 64; i++) {
        g_vertex_buffer_data[i][0] = _vertices[i][0];
        g_vertex_buffer_data[i][1] = _vertices[i][1];
        g_vertex_buffer_data[i][2] = _vertices[i][2];
        g_uv_buffer_data[i][0] = (g_uv_buffer_data[i][0] + 1) / 2;
        g_uv_buffer_data[i][1] = (g_uv_buffer_data[i][1] + 1) / 2;
    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    do{

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
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

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}