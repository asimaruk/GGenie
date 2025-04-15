#include <GLFW/glfw3.h>
#include "GlfwEngineWindow.h"

GlfwEngineWindow::~GlfwEngineWindow() {
    glfwTerminate();
}

void GlfwEngineWindow::initialize(int width, int height, const char* title) {
    // GLFW initialization
    if (!glfwInit()) {
        throw "GLFW error!";
    }

    // OpenGL 3.3 setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Важно для macOS!

    // Window creation
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw "Window creation error!";
    }

    glfwMakeContextCurrent(window);
}

void GlfwEngineWindow::pollEvents() {
    glfwPollEvents();
}

void GlfwEngineWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

bool GlfwEngineWindow::shouldClose() {
    return glfwWindowShouldClose(window);
}
