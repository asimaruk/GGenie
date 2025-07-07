#include "system/input/GLFWInputSystem.h"
#include "ecs/System.h"
#include "system/event/EventSystem.hpp"
#include "system/input/GLFWInputEvent.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <memory>
#include <utility>

class GLFWInputSystem::Impl {
private:
  std::shared_ptr<EventSystem> eventSystem;

  void keyCallback(GLFWwindow * /*window*/, int key, int scancode, int action, int mods) {
    eventSystem->dispatch(GLFWKeyboardEvent{.key = key, .scancode = scancode, .action = action, .mods = mods});
  }

  void mouseButtonCallback(GLFWwindow * /*window*/, int button, int action, int mods) {
    eventSystem->dispatch(GLFWMouseButtonEvent{.button = button, .action = action, .mods = mods});
  }

  void cursorPosCallback(GLFWwindow * /*window*/, double xpos, double ypos) {
    eventSystem->dispatch(GLFWCursorPositionEvent{.xpos = xpos, .ypos = ypos});
  }

  void cursorEnterCallback(GLFWwindow * /*window*/, int entered) {
    eventSystem->dispatch(GLFWCursorEnterEvent{.entered = entered});
  }

  void scrollCallback(GLFWwindow * /*window*/, double xoffset, double yoffset) {
    eventSystem->dispatch(GLFWScrollEvent{.xoffset = xoffset, .yoffset = yoffset});
  }

public:
  explicit Impl(std::shared_ptr<EventSystem> eventSystem) noexcept : eventSystem(std::move(eventSystem)) {}

  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;
  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  ~Impl() {
    auto *glfwWindow = glfwGetCurrentContext();
    if (glfwWindow != nullptr) {
      glfwSetWindowUserPointer(glfwWindow, nullptr);
    }
  }

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
};

GLFWInputSystem::GLFWInputSystem(SystemID id, int priority, std::shared_ptr<EventSystem> eventSystem) noexcept
    : System(id, priority)
    , pimpl(std::make_unique<Impl>(eventSystem)) {}

GLFWInputSystem::~GLFWInputSystem() = default;

void GLFWInputSystem::start() {
  auto *glfwWindow = glfwGetCurrentContext();
  assert(glfwWindow != nullptr && "GLFWWindow* is null");
  glfwSetWindowUserPointer(glfwWindow, this->pimpl.get());
  glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide cursor for infinite cursor moves
  glfwSetKeyCallback(glfwWindow, Impl::keyCallbackWrapper);
  glfwSetMouseButtonCallback(glfwWindow, Impl::mouseButtonCallbackWrapper);
  glfwSetCursorEnterCallback(glfwWindow, Impl::cursorEnterCallbackWrapper);
  glfwSetCursorPosCallback(glfwWindow, Impl::cursorPosCallbackWrapper);
  glfwSetScrollCallback(glfwWindow, Impl::scrollCallbackWrapper);
}

void GLFWInputSystem::update(float /*dt*/) {
  glfwPollEvents();
}
