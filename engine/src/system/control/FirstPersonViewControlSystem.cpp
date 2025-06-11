#include "system/control/FirstPersonViewControlSystem.h"
#include "component/Move.h"
#include "component/Transform.h"
#include "ecs/System.h"
#include "math/Vec3.h"
#include "system/event/Event.h"
#include "system/input/GLFWInputEvent.h"
#include <GLFW/glfw3.h>
#include <numbers>
#include <print>

class FirstPersonViewControlSystem::Impl {
private:
  std::shared_ptr<ComponentRegistry> registry;
  Entity controlledEntity = -1;
  Vec3 direction = Vec3::ZERO;
  Vec3 cursorPosition = Vec3::ZERO;
  Vec3 lastCursorPosition = Vec3::ZERO;

public:
  Impl(std::shared_ptr<ComponentRegistry> registry) : registry(registry) {}

  void setControlledEntity(Entity entity) noexcept {
    controlledEntity = entity;
  }

  void update(float dt) {
    const auto transform = registry->get<Transform>(controlledEntity);
    const auto move = registry->get<Move>(controlledEntity);
    const auto cursorPosDiff = cursorPosition - lastCursorPosition;

    if (transform && move && (direction != Vec3::ZERO || cursorPosDiff != Vec3::ZERO)) {
      registry->replace<Transform>(
          controlledEntity,
          Transform{
              .translation = transform->get().translation + direction * move->get().speed * dt,
              .rotation =
                  transform->get().rotation * Quat::fromEuler(cursorPosDiff.x / 1000, 0, cursorPosDiff.y / 1000),
              .scale = transform->get().scale,
          }
      );
      lastCursorPosition = cursorPosition;
    }
  }

  void onGLFWCursorEnterEvent(const GLFWCursorEnterEvent &event) {
    std::println("FirstPersonViewControlSystem::onGLFWCursorEnterEvent {{ entered={} }}", event.entered != 0);
    cursorPosition = lastCursorPosition = Vec3::ZERO;
  }

  void onGLFWCursorPositionEvent(const GLFWCursorPositionEvent &event) {
    std::println(
        "FirstPersonViewControlSystem::onGLFWCursorPositionEvent {{ xpos={:.2f}, ypos={:.2f} }}",
        event.xpos,
        event.ypos
    );
    auto pos = Vec3{.x = static_cast<float>(event.xpos), .y = static_cast<float>(event.ypos), .z = 0};
    if (cursorPosition == Vec3::ZERO && lastCursorPosition == Vec3::ZERO) {
      cursorPosition = lastCursorPosition = pos;
    } else {
      cursorPosition = pos;
    }
  }

  void onGLFWKeyboardEvent(const GLFWKeyboardEvent &event) {
    std::println(
        "FirstPersonViewControlSystem::onGLFWKeyboardEvent {{ key={}, scancode={}, action={}, mods={} }}",
        event.key,
        event.scancode,
        event.action,
        event.mods
    );

    Vec3 keyboardDirection = Vec3::ZERO;
    switch (event.key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
      keyboardDirection += Vec3{0, 0, 1};
      break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
      keyboardDirection += Vec3{0, 0, -1};
      break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
      keyboardDirection += Vec3{1, 0, 0};
      break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
      keyboardDirection += Vec3{-1, 0, 0};
      break;
    }

    switch (event.action) {
    case GLFW_PRESS:
      direction += keyboardDirection;
      break;
    case GLFW_RELEASE:
      direction -= keyboardDirection;
      break;
    }
  }

  void onGLFWMouseButtonEvent(const GLFWMouseButtonEvent &event) {
    std::println(
        "FirstPersonViewControlSystem::onGLFWMouseButtonEvent {{ button={}, action={}, mods={} }}",
        event.button,
        event.action,
        event.mods
    );
  }

  void onGLFWScrollEvent(const GLFWScrollEvent &event) {
    std::println(
        "FirstPersonViewControlSystem::onGLFWCursorPositionEvent {{ xoffset={:.2f}, yoffset={:.2f} }}",
        event.xoffset,
        event.yoffset
    );
  }
};

FirstPersonViewControlSystem::FirstPersonViewControlSystem(
    SystemID systemId,
    int priority,
    std::shared_ptr<ComponentRegistry> registry,
    std::shared_ptr<EventSystem> eventSystem
)
    : System(systemId, priority)
    , pimpl(new Impl(registry)) {
  eventSystem->on<GLFWCursorPositionEvent>(
      [this](const GLFWCursorPositionEvent &event)
      {
        pimpl->onGLFWCursorPositionEvent(event);
      }
  );
  eventSystem->on<GLFWKeyboardEvent>(
      [this](const GLFWKeyboardEvent &event)
      {
        pimpl->onGLFWKeyboardEvent(event);
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

void FirstPersonViewControlSystem::setControlledEntity(Entity entity) noexcept {
  pimpl->setControlledEntity(entity);
}

void FirstPersonViewControlSystem::start() {}

void FirstPersonViewControlSystem::update(float dt) {
  pimpl->update(dt);
}

FirstPersonViewControlSystem::~FirstPersonViewControlSystem() {}
