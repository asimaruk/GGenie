#pragma once

#include "ecs/System.h"
#include <GLFW/glfw3.h>
#include <memory>

class GLFWInputSystem final : public System {
private:
  std::shared_ptr<GLFWwindow> window;
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  static constexpr auto ID = "GLFWInputSystemID";
  GLFWInputSystem(SystemID id, int priority, std::shared_ptr<GLFWwindow> glfwWindow) noexcept;
  ~GLFWInputSystem();
  virtual void start() override;
  virtual void update(float dt) override;
};
