#include "system/input/GLFWInputSystem.h"
#include "ecs/System.h"
#include <__ostream/print.h>
#include <format>
#include <iostream>
#include <string_view>

static std::string_view getActionName(int action) {
    return action == GLFW_PRESS ? "PRESSED" : action == GLFW_REPEAT ? "REPEATED" : "RELEASED";
}

class GLFWInputSystem::Impl {
private:
  std::shared_ptr<GLFWwindow> glfwWindow;

  static void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->keyCallback(window, key, scancode, action, mods);
  }

  static void mouseButtonCallbackWrapper(GLFWwindow *window, int button, int action, int mods) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->mouseButtonCallback(window, button, action, mods);
  }

  static void cursorPosCallbackWrapper(GLFWwindow *window, double xpos, double ypos) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->cursorPosCallback(window, xpos, ypos);
  }

  static void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->scrollCallback(window, xoffset, yoffset);
  }

  void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto keyName = glfwGetKeyName(key, scancode);
    if (keyName == NULL) {
        keyName = std::format("token {} scancode {}", key, scancode).data();
    }
    auto actionName = getActionName(action);
    std::println("{}", std::format("GLFWInputSystem:: key {} {} with mods {}", keyName, actionName, mods));
  }

  void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    auto actionName = getActionName(action);
    std::println("{}", std::format("GLFWInputSystem:: mouse button {} {} with mods {}", button, actionName, mods));
  }

  void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    std::println("{}", std::format("GLFWInputSystem:: Cursor position {{ {:.2f}; {:.2f} }}", xpos, ypos));
  }

  void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    std::println("{}", std::format("GLFWInputSystem:: Scroll offset {{ {:.2f}; {:.2f} }}", xoffset, yoffset));
  }

public:
  Impl(std::shared_ptr<GLFWwindow> glfwWindow) : glfwWindow(glfwWindow) {
    auto glfwWindowPtr = glfwWindow.get();
    if (!glfwWindowPtr) {
      std::println(std::cerr, "GLFWWindow* is null in GLFWInputSystem::Impl()");
    }

    glfwMakeContextCurrent(glfwWindowPtr);
    glfwSetWindowUserPointer(glfwWindowPtr, this);
    glfwSetKeyCallback(glfwWindowPtr, Impl::keyCallbackWrapper);
    glfwSetMouseButtonCallback(glfwWindowPtr, Impl::mouseButtonCallbackWrapper);
    glfwSetCursorPosCallback(glfwWindowPtr, Impl::cursorPosCallbackWrapper);
    glfwSetScrollCallback(glfwWindowPtr, Impl::scrollCallbackWrapper);
  }

  ~Impl() { glfwSetWindowUserPointer(glfwWindow.get(), nullptr); }
};

GLFWInputSystem::GLFWInputSystem(SystemID id, int priority, std::shared_ptr<GLFWwindow> glfwWindow) noexcept
    : System(id, priority), window(glfwWindow), pimpl(new Impl(glfwWindow)) {}

GLFWInputSystem::~GLFWInputSystem() = default;

void GLFWInputSystem::start() { std::println("GLFWInputSystem.start()"); }

void GLFWInputSystem::update(float dt) { glfwPollEvents(); }
