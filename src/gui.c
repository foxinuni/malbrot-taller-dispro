#include <stdio.h>
#include <stdlib.h>

// Include opengl and glfw and glew
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/vector.h"
#include "math/complex.h"
#include "math/matrix.h"

#define WIDTH 1280
#define HEIGHT 720

GLuint compile_shader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        char* log = (char*)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, log);

        char* shaderTypeStr = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";

        printf("Failed to compile \"%s\" shader! %s\n", shaderTypeStr, log);
        free(log);

        return 0;
    }

    return shader;
}

GLuint create_program(const char* vertexSource, const char* fragmentSource) {
    GLuint program = glCreateProgram();

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        return 0;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        char* log = (char*)malloc(logLength);
        glGetProgramInfoLog(program, logLength, NULL, log);

        printf("Failed to link program! %s\n", log);
        free(log);

        return 0;
    }

    return program;
}

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    // get file length in bytes
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory and read file
    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    // close the file descriptor
    fclose(file);

    return buffer;
}

int main(int argc, char* argv[]) {
    // Initialize glfw
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize glew
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    // vertex buffer
    float vertices[] = {
        -1.0f, -1.0f, // bottom left
        1.0f, -1.0f,  // bottom right
        -1.0f, 1.0f,  // top left
        1.0f, 1.0f   // top right
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    // vertex arrays, attribs and buffers
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the vertex array object
    glBindVertexArray(vao);

    // bind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // create the attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo
    glBindVertexArray(0); // vba

    // create the shader program
    char* vertexSource = read_file("shaders/malbrot_vertex.glsl");
    if (!vertexSource) {
        printf("Failed to read vertex shader source!\n");
        return -1;
    }

    char* fragmentSource = read_file("shaders/malbrot_fragment.glsl");
    if (!fragmentSource) {
        printf("Failed to read fragment shader source!\n");
        return -1;
    }

    // create the shader program
    GLuint program = create_program(vertexSource, fragmentSource);
    if (!program) {
        printf("Failed to create shader program!\n");
        return -1;
    }

    // free the source code
    free(vertexSource);
    free(fragmentSource);

    float zoom = 1.0f;
    float center_x = 0.0f;
    float center_y = 0.0f;
    float zoom_speed = 0.1f;
    float move_speed = 0.1f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            center_y += move_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            center_y -= move_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            center_x -= move_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            center_x += move_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            zoom += zoom_speed;
            move_speed = 0.1f / zoom;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            zoom -= zoom_speed;
            move_speed = 0.1f / zoom;
        }

        // use the shader program
        glUseProgram(program);

        // set uniforms
        glUniform2f(glGetUniformLocation(program, "u_resolution"), WIDTH, HEIGHT);
        glUniform2f(glGetUniformLocation(program, "u_center"), center_x, center_y);
        glUniform1f(glGetUniformLocation(program, "u_zoom"), zoom);

        // bind the vertex array object
        glBindVertexArray(vao);

        // draw the elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // poll events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate glfw
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}