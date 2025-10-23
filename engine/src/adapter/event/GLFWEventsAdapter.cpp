#include "adapter/event/GLFWEventsAdapter.h"
#include "math/algebras.h"
#include "system/control/FPVEvent.h"
#include "system/event/EventSystem.hpp"
#include "system/input/GLFWInputEvent.h"
#include <GLFW/glfw3.h>
#include <format>
#include <memory>
#include <print>
#include <string>

using namespace GGenie;

class GLFWEventsAdapter::Impl {
private:
  Vec3 lastCursorPosition = Vec3::ZERO;
  Vec3 direction = Vec3::ZERO;
  bool isLogOn;

  void log(const std::string message) {
    if (!isLogOn) {
      return;
    }
    std::println("{}", message);
  }

public:
  Impl(const bool logging) : isLogOn(logging) {}

  void onGLFWCursorEnterEvent(const GLFWCursorEnterEvent &event) {
    log(std::format("GLFWEventsAdapter::onGLFWCursorEnterEvent {{ entered={} }}", event.entered != 0));
    lastCursorPosition = Vec3::ZERO;
  }

  auto onGLFWCursorPositionEvent(const GLFWCursorPositionEvent &event) -> FPVViewEvent {
    log(std::format(
        "GLFWEventsAdapter::onGLFWCursorPositionEvent {{ xpos={:.2f}, ypos={:.2f} }}",
        event.xpos,
        event.ypos
    ));
    auto pos = Vec3{.x = static_cast<float>(event.xpos), .y = static_cast<float>(event.ypos), .z = 0};
    if (lastCursorPosition == Vec3::ZERO) {
      lastCursorPosition = pos;
    }

    auto viewEvent = FPVViewEvent{.xViewDiff = pos.x - lastCursorPosition.x, .yViewDiff = pos.y - lastCursorPosition.y};
    lastCursorPosition = pos;
    return viewEvent;
  }

  auto onGLFWKeyboardEvent(const GLFWKeyboardEvent &event) -> FPVMoveEvent {
    log(std::format(
        "GLFWEventsAdapter::onGLFWKeyboardEvent {{ key={}, scancode={}, action={}, mods={} }}",
        event.key,
        event.scancode,
        event.action,
        event.mods
    ));

    Vec3 keyboardDirection = Vec3::ZERO;
    switch (event.key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
      keyboardDirection += Vec3{.x = 0, .y = 0, .z = -1};
      break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
      keyboardDirection += Vec3{.x = 0, .y = 0, .z = 1};
      break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
      keyboardDirection += Vec3{.x = -1, .y = 0, .z = 0};
      break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
      keyboardDirection += Vec3{.x = 1, .y = 0, .z = 0};
      break;
    default:
      break;
    }

    switch (event.action) {
    case GLFW_PRESS:
      direction += keyboardDirection;
      break;
    case GLFW_RELEASE:
      direction -= keyboardDirection;
      break;
    default:
      break;
    }

    return FPVMoveEvent{.direction = direction};
  }

  void onGLFWMouseButtonEvent(const GLFWMouseButtonEvent &event) {
    log(std::format(
        "GLFWEventsAdapter::onGLFWMouseButtonEvent {{ button={}, action={}, mods={} }}",
        event.button,
        event.action,
        event.mods
    ));
  }

  void onGLFWScrollEvent(const GLFWScrollEvent &event) {
    log(std::format(
        "GLFWEventsAdapter::onGLFWCursorPositionEvent {{ xoffset={:.2f}, yoffset={:.2f} }}",
        event.xoffset,
        event.yoffset
    ));
  }
};

GLFWEventsAdapter::GLFWEventsAdapter(const bool logging) : pimpl(new Impl(logging)) {}

void GLFWEventsAdapter::attach(std::shared_ptr<EventSystem> eventSystem) {
  eventSystem->on<GLFWCursorPositionEvent>(
      [this, eventSystem](const GLFWCursorPositionEvent &event)
      {
        eventSystem->dispatch(pimpl->onGLFWCursorPositionEvent(event));
      }
  );
  eventSystem->on<GLFWKeyboardEvent>(
      [this, eventSystem](const GLFWKeyboardEvent &event)
      {
        eventSystem->dispatch(pimpl->onGLFWKeyboardEvent(event));
      }
  );
  eventSystem->on<GLFWMouseButtonEvent>(
      [this](const GLFWMouseButtonEvent &event)
      {
        pimpl->onGLFWMouseButtonEvent(event);
      }
  );
  eventSystem->on<GLFWScrollEvent>(
      [this](const GLFWScrollEvent &event)
      {
        pimpl->onGLFWScrollEvent(event);
      }
  );
  eventSystem->on<GLFWCursorEnterEvent>(
      [this](const GLFWCursorEnterEvent &event)
      {
        pimpl->onGLFWCursorEnterEvent(event);
      }
  );
}

void GLFWEventsAdapter::detach(std::shared_ptr<EventSystem> eventSystem) {
  // not implemented
}

GLFWEventsAdapter::~GLFWEventsAdapter() = default;
