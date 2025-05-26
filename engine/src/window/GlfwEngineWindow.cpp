#include "window/GlfwEngineWindow.h"
#include <GLFW/glfw3.h>
#include <memory>

GlfwEngineWindow::~GlfwEngineWindow() { glfwTerminate(); }

void GlfwEngineWindow::initialize(int width, int height, const char *title) {
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
  window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, title, NULL, NULL),
                                       [](GLFWwindow *w) { glfwDestroyWindow(w); });
  if (!window) {
    glfwTerminate();
    throw "Window creation error!";
  }

  glfwMakeContextCurrent(window.get());
}

void GlfwEngineWindow::swapBuffers() { glfwSwapBuffers(window.get()); }

bool GlfwEngineWindow::shouldClose() { return glfwWindowShouldClose(window.get()); }

std::shared_ptr<GLFWwindow> GlfwEngineWindow::getGlfwWindow() const { return window; }
