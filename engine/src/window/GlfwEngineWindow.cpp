#include "window/GlfwEngineWindow.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <stdexcept>

using namespace GGenie;

GlfwEngineWindow::GlfwEngineWindow() = default;

GlfwEngineWindow::~GlfwEngineWindow() {
  if (window != nullptr) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void GlfwEngineWindow::initialize(int width, int height, const char *title) {
  // GLFW initialization
  if (glfwInit() == 0) {
    throw std::runtime_error("GLFW error!");
  }

  // OpenGL 3.3 setup
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Important for macOS!

  // Window creation
  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    throw std::runtime_error("Window creation error!");
  }

  glfwMakeContextCurrent(window);
}

void GlfwEngineWindow::swapBuffers() {
  if (window != nullptr) {
    glfwSwapInterval(0); // immediate swapping fpr unlimited FPS
    glfwSwapBuffers(window);
  }
}

auto GlfwEngineWindow::shouldClose() const -> bool {
  if (window == nullptr) {
    return true;
  }
  return glfwWindowShouldClose(window) != 0;
}

auto GlfwEngineWindow::getGlfwWindow() const -> GLFWwindow * {
  return window;
}

auto GlfwEngineWindow::getWidth() const -> int {
  if (window == nullptr) {
    return 0;
  }
  int width = 0;
  glfwGetWindowSize(window, &width, nullptr);
  return width;
}

auto GlfwEngineWindow::getHeight() const -> int {
  if (window == nullptr) {
    return 0;
  }
  int height = 0;
  glfwGetWindowSize(window, nullptr, &height);
  return height;
}
