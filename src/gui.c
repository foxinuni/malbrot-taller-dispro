#include <stdio.h>
#include <stdlib.h>

// Include opengl and glfw and glew
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/vector.h"
#include "math/complex.h"
#include "math/matrix.h"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* argv[]) {
    // Initialize glfw
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Malbrot", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize glew
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    // Start the main loop
    double position[2] = { 0.0, 0.0 };
    double zoom = 1.0;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate glfw
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}