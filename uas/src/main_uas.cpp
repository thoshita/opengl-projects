
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <commons/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <commons/texture.h>
#include <commons/objloader.h>
#include <commons/particle.h>
#include <algorithm>
#include <commons/stb_image.h>

using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SIDES_OF_CIRCLE 100
#define NUM_OF_WHEELS 5
#define VERTEX_SHADER_FILE_PATH  "uas/shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE_PATH "uas/shaders/fragment_shader.glsl"
#define PARTICLE_VERTEX_SHADER_FILE_PATH  "uas/shaders/particle_vertex_shader.glsl"
#define PARTICLE_FRAGMENT_SHADER_FILE_PATH "uas/shaders/particle_fragment_shader.glsl"
#define BITMAP_PATH "uas/batik.bmp"
#define SMOKE_PNG_PATH "uas/smoke.png"
#define RAIN_PNG_PATH "uas/rain.png"

GLfloat y_min = 999;

const int MaxParticles = 50000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0, ParticlesCount = 0;

static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
static GLubyte* g_particule_color_data         = new GLubyte[MaxParticles * 4];

int FindUnusedParticle(){
    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

void SortParticles(){
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

void genParticles(double delta) {
    int newparticles = (int)(delta*1000.0);
    if (newparticles > (int)(0.016f*1000.0))
        newparticles = (int)(0.016f*1000.0);

    for(int i=0; i<newparticles; i++){
        int particleIndex = FindUnusedParticle();

        if (i % 10 != 0) {
            // Smoke
            ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
            ParticlesContainer[particleIndex].pos = glm::vec3(0, 0.5, -3);

            float spread = 1.5f;
            glm::vec3 maindir = glm::vec3(0.0f, 0.0f, -3.0f);

            glm::vec3 randomdir = glm::vec3(
                    (rand() % 2000 - 1000.0f) / 2000.0f,
                    (rand() % 2000) / 5000.0f,
                    (rand() % 2000 - 1000.0f) / 1000.0f
            );

            ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;

            // Type: 0 - smoke
            ParticlesContainer[particleIndex].r = 0;

            ParticlesContainer[particleIndex].size = (rand() % 1000) / 4000.0f + 0.3f;
        }
        else {
            // Rain
            ParticlesContainer[particleIndex].life = 1000;
            ParticlesContainer[particleIndex].pos = glm::vec3((rand() % 20000000 - 10000000) / 100000.0f / 7,
                                                               100,
                                                              (rand() % 20000000 - 10000000) / 100000.0f / 7);

            ParticlesContainer[particleIndex].speed = glm::vec3(0, - rand() % 10000 / 10.0f, 0);

            // Type: 1 - rain
            ParticlesContainer[particleIndex].r = 1;

            ParticlesContainer[particleIndex].size = 0.1;
        }
    }
}

void drawImageTexture(GLint texture, GLenum format, GLint width, GLint height, unsigned char* data) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void simulateParticles(double delta, glm::vec3 CameraPosition) {
// Simulate all particles
    ParticlesCount = 0;
    for(int i=0; i<MaxParticles; i++){

        Particle& p = ParticlesContainer[i]; // shortcut

        if(p.life > 0.0f){

            // Decrease life
            p.life -= delta;
            if (p.life > 0.0f && p.pos.y >= y_min){

                // Simulate simple physics : gravity only, no collisions
                p.speed += glm::vec3(0.0f,p.r == 0 ? (rand() % 5) / 5.0f : -9.8f, 0.0f) * (float)delta * 0.5f;
                p.pos += p.speed * (float)delta;
                p.cameradistance = glm::length2( p.pos - CameraPosition );
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                g_particule_color_data[4*ParticlesCount+0] = p.r;
                g_particule_color_data[4*ParticlesCount+1] = p.g;
                g_particule_color_data[4*ParticlesCount+2] = p.b;
                g_particule_color_data[4*ParticlesCount+3] = p.a;

            }else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameradistance = -1.0f;
                p.life = -1.0f;
            }

            ParticlesCount++;

        }
    }

    SortParticles();
}

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "UAS", nullptr, nullptr);
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

    for(int i=0; i<MaxParticles; i++){
        ParticlesContainer[i].life = -1.0f;
        ParticlesContainer[i].cameradistance = -1.0f;
    }

    GLuint shader_program = LoadShaders(VERTEX_SHADER_FILE_PATH, FRAGMENT_SHADER_FILE_PATH);
    GLuint particle_shader_program = LoadShaders(PARTICLE_VERTEX_SHADER_FILE_PATH, PARTICLE_FRAGMENT_SHADER_FILE_PATH);

    GLuint matrix = glGetUniformLocation(shader_program, "MVP");
    GLuint camera_right_particle  = glGetUniformLocation(particle_shader_program, "CameraRight_worldspace");
    GLuint camera_up_particle  = glGetUniformLocation(particle_shader_program, "CameraUp_worldspace");
    GLuint matrix_particle = glGetUniformLocation(particle_shader_program, "VP");

    GLuint Texture = loadBMP_custom(BITMAP_PATH);
    GLuint TextureID  = glGetUniformLocation(shader_program, "myTexture");
    GLuint smoke_texture_id  = glGetUniformLocation(particle_shader_program, "smokeTexture");
    GLuint rain_texture_id  = glGetUniformLocation(particle_shader_program, "rainTexture");

    glm::mat4 Projection = glm::perspective(glm::radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View;
    glm::mat4 Model      = glm::mat4(1.0f);
    glm::mat4 MVP;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uv_vertices;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ("uas/car.obj", vertices, uv_vertices, normals);

    for (glm::vec3 vertex : vertices) {
        y_min = min(y_min, vertex.y);
    }

    static const GLfloat terrain_vertices[] = {
            -1000.0f, y_min, -1000.0f,
            1000.0f, y_min,  -1000.0f,
            -1000.0f, y_min, 1000.0f,

            1000.0f, y_min,  -1000.0f,
            -1000.0f, y_min, 1000.0f,
            1000.0f, y_min, 1000.0f
    };

    static const GLfloat terrain_uv[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,

            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f
    };

    for (int i = 0; i < 6; i++) {
        vertices.push_back(glm::vec3(terrain_vertices[i*3], terrain_vertices[i*3 + 1], terrain_vertices[i*3 + 2]));
        uv_vertices.push_back(glm::vec2(terrain_uv[i*2], terrain_uv[i*2 + 1]));
    }

    static const GLfloat g_rain_buffer_data[] = {
            -0.1f, -1.f, 0.0f,
            0.1f, -1.f, 0.0f,
            -0.1f,  1.f, 0.0f,
            0.1f,  1.f, 0.0f,
    };

    static const GLfloat g_vertex_buffer_data[] = {
            -1.f, -1.f, 0.0f,
            -1.f,  1.f, 0.0f,
            1.f, -1.f, 0.0f,
            1.f,  1.f, 0.0f,
    };

    GLuint particle_vertex_array;
    glGenVertexArrays(1, &particle_vertex_array);
    glBindVertexArray(particle_vertex_array);

    GLuint rain_vertex_buffer;
    glGenBuffers(1, &rain_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, rain_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_rain_buffer_data), g_rain_buffer_data, GL_STATIC_DRAW);

    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uv_buffer;
    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, uv_vertices.size() * sizeof(glm::vec2), &uv_vertices[0], GL_STATIC_DRAW);
    GLuint smoke_texture;
    glGenTextures(1, &smoke_texture);
    int smoke_width, smoke_height, n_components;
    unsigned char* smoke_data = stbi_load(SMOKE_PNG_PATH, &smoke_width, &smoke_height, &n_components, 0);
    GLenum smoke_format;

    switch (n_components) {
        case 1:
            smoke_format = GL_RED;
            break;
        case 3:
            smoke_format = GL_RGB;
            break;
        case 4:
            smoke_format = GL_RGBA;
            break;
    }

    GLuint rain_texture;
    glGenTextures(1, &rain_texture);
    int rain_width, rain_height;
    unsigned char* rain_data = stbi_load(RAIN_PNG_PATH, &rain_width, &rain_height, &n_components, 0);
    GLenum rain_format;

    switch (n_components) {
        case 1:
            rain_format = GL_RED;
            break;
        case 3:
            rain_format = GL_RGB;
            break;
        case 4:
            rain_format = GL_RGBA;
            break;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLfloat camera_x = -10, camera_y = 4, camera_z = 10;
    GLfloat direction_x = 10, direction_y = -4, direction_z = -10;
    GLfloat acceleration = 0.1;

    double lastTime = glfwGetTime();

    do {
        camera_y = max(camera_y, y_min + 0.1f);
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        lastTime = currentTime;

        printf("%.2f\n", 1.0f / delta);
        genParticles(delta);
        simulateParticles(delta, glm::vec3(camera_x, camera_y, camera_z));

        View = glm::lookAt(
                glm::vec3(camera_x,camera_y,camera_z),
                glm::vec3(camera_x + direction_x, camera_y + direction_y, camera_z + direction_z),
                glm::vec3(0,1,0)
        );
        MVP = Projection * View * Model;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glBindVertexArray(vertex_array);
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
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);







        glBindVertexArray(particle_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use our shader
        glUseProgram(particle_shader_program);

        glActiveTexture(GL_TEXTURE1);
        drawImageTexture(smoke_texture, smoke_format, smoke_width, smoke_height, smoke_data);
        glUniform1i(smoke_texture_id, 1);

        glActiveTexture(GL_TEXTURE2);
        drawImageTexture(rain_texture, rain_format, rain_width, rain_height, rain_data);
        glUniform1i(rain_texture_id, 2);

        // Same as the billboards tutorial
        glUniform3f(camera_right_particle, View[0][0], View[1][0], View[2][0]);
        glUniform3f(camera_up_particle, View[0][1], View[1][1], View[2][1]);

        glUniformMatrix4fv(matrix_particle, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : x + y + z + size => 4
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(
                2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : r + g + b + a => 4
                GL_UNSIGNED_BYTE,                 // type
                GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // 4th attribute buffer : rain vertices
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, rain_vertex_buffer);
        glVertexAttribPointer(
                3,                  // attribute. No particular reason for 3, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // These functions are specific to glDrawArrays*Instanced*.
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
        // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1
        glVertexAttribDivisor(3, 0); // particles vertices : always reuse the same 4 vertices -> 0

        // Draw the particules !
        // This draws many times a small triangle_strip (which looks like a quad).
        // This is equivalent to :
        // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
        // but faster.
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);

        glDisable(GL_BLEND);

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