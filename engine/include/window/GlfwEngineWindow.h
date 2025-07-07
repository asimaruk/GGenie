#ifndef WINDOW_GLFWENGINEWINDOW_H
#define WINDOW_GLFWENGINEWINDOW_H

#include "EngineWindow.h"
#include <GLFW/glfw3.h>

class GlfwEngineWindow : public EngineWindow {
private:
  GLFWwindow *window{nullptr};

public:
  GlfwEngineWindow();
  GlfwEngineWindow(const GlfwEngineWindow &) = delete;
  GlfwEngineWindow(GlfwEngineWindow &&) = delete;
  ~GlfwEngineWindow() override;

  auto operator=(const GlfwEngineWindow &) -> GlfwEngineWindow & = delete;
  auto operator=(GlfwEngineWindow &&) -> GlfwEngineWindow & = delete;

  void initialize(int width, int height, const char *title) override;
  void swapBuffers() override;
  [[nodiscard]] auto shouldClose() const -> bool override;
  [[nodiscard]] auto getWidth() const -> int override;
  [[nodiscard]] auto getHeight() const -> int override;
  [[nodiscard]] auto getGlfwWindow() const -> GLFWwindow *;
};

#endif
