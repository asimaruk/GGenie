#pragma once

#include "EngineWindow.h"
#include <GLFW/glfw3.h>
#include <memory>

class GlfwEngineWindow : public EngineWindow {
private:
  std::shared_ptr<GLFWwindow> window;

public:
  ~GlfwEngineWindow() override;
  void initialize(int width, int height, const char *title) override;
  void swapBuffers() override;
  bool shouldClose() override;
  std::shared_ptr<GLFWwindow> getGlfwWindow() const;
};