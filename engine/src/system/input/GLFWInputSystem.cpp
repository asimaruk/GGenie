#include "system/input/GLFWInputSystem.h"
#include "ecs/System.h"
#include "system/event/Event.h"
#include "system/input/GLFWInputEvent.h"
#include <cstddef>
#include <format>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <print>
#include <string_view>
#include <utility>

class GLFWInputSystem::Impl {
private:
  std::shared_ptr<EventSystem> eventSystem;

  static void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->keyCallback(window, key, scancode, action, mods);
  }

  static void mouseButtonCallbackWrapper(GLFWwindow *window, int button, int action, int mods) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->mouseButtonCallback(window, button, action, mods);
  }

  static void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->cursorEnterCallback(window, entered);
  }

  static void cursorPosCallbackWrapper(GLFWwindow *window, double xpos, double ypos) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->cursorPosCallback(window, xpos, ypos);
  }

  static void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
    auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(window));
    self->scrollCallback(window, xoffset, yoffset);
  }

  void keyCallback(GLFWwindow * /*window*/, int key, int scancode, int action, int mods) {
    const auto *keyName = glfwGetKeyName(key, scancode);
    if (keyName == nullptr) {
      keyName = std::format("token {} scancode {}", key, scancode).data();
    }
    eventSystem->dispatch(GLFWKeyboardEvent{.key = key, .scancode = scancode, .action = action, .mods = mods});
  }

  void mouseButtonCallback(GLFWwindow * /*window*/, int button, int action, int mods) {
    eventSystem->dispatch(GLFWMouseButtonEvent{.button = button, .action = action, .mods = mods});
  }

  void cursorPosCallback(GLFWwindow * /*window*/, double xpos, double ypos) {
    eventSystem->dispatch(GLFWCursorPositionEvent{.xpos = xpos, .ypos = ypos});
  }

  void cursorEnterCallback(GLFWwindow *window, int entered) {
    eventSystem->dispatch(GLFWCursorEnterEvent{.entered = entered});
  }

  void scrollCallback(GLFWwindow * /*window*/, double xoffset, double yoffset) {
    eventSystem->dispatch(GLFWScrollEvent{.xoffset = xoffset, .yoffset = yoffset});
  }

public:
  explicit Impl(GLFWwindow *glfwWindow, std::shared_ptr<EventSystem> eventSystem) noexcept : eventSystem(eventSystem) {
    if (glfwWindow == nullptr) {
      std::println(std::cerr, "GLFWWindow* is null in GLFWInputSystem::Impl()");
      return;
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide cursor for infinite cursor moves

    glfwSetKeyCallback(glfwWindow, Impl::keyCallbackWrapper);
    glfwSetMouseButtonCallback(glfwWindow, Impl::mouseButtonCallbackWrapper);
    glfwSetCursorEnterCallback(glfwWindow, Impl::cursorEnterCallbackWrapper);
    glfwSetCursorPosCallback(glfwWindow, Impl::cursorPosCallbackWrapper);
    glfwSetScrollCallback(glfwWindow, Impl::scrollCallbackWrapper);
  }

  ~Impl() {
    auto *glfwWindow = glfwGetCurrentContext();
    if (glfwWindow != nullptr) {
      glfwSetWindowUserPointer(glfwWindow, nullptr);
    }
  }
};

GLFWInputSystem::GLFWInputSystem(
    SystemID id, int priority, GLFWwindow *glfwWindow, std::shared_ptr<EventSystem> eventSystem
) noexcept
    : System(id, priority)
    , pimpl(new Impl(glfwWindow, eventSystem)) {}

GLFWInputSystem::~GLFWInputSystem() = default;

void GLFWInputSystem::start() {
  std::println("GLFWInputSystem.start()");
}

void GLFWInputSystem::update(float /*dt*/) {
  glfwPollEvents();
}
