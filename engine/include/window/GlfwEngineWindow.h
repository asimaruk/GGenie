#pragma once

#include "EngineWindow.h"
#include <GLFW/glfw3.h>

class GlfwEngineWindow : public EngineWindow {
private:
  GLFWwindow *window;

public:
  ~GlfwEngineWindow() override;
  void initialize(int width, int height, const char *title) override;
  void swapBuffers() override;
  bool shouldClose() override;
  GLFWwindow *getGlfwWindow() const;
};