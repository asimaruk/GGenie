#ifndef SYSTEM_INPUT_GLFWINPUTSYSTEM_H
#define SYSTEM_INPUT_GLFWINPUTSYSTEM_H

#include "ecs/System.h"
#include "system/event/EventSystem.hpp"
#include <GLFW/glfw3.h>
#include <memory>

namespace GGenie {

class GLFWInputSystem final : public System {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  static constexpr auto ID = "GLFWInputSystemID";
  GLFWInputSystem(SystemID id, int priority, std::shared_ptr<EventSystem> eventSystem) noexcept;
  GLFWInputSystem(const GLFWInputSystem &) = delete;
  GLFWInputSystem(GLFWInputSystem &&) = delete;
  ~GLFWInputSystem() override;

  auto operator=(const GLFWInputSystem &) -> GLFWInputSystem & = delete;
  auto operator=(GLFWInputSystem &&) -> GLFWInputSystem & = delete;

  void start() override;
  void update(float dt) override;
};

} // namespace GGenie

#endif
