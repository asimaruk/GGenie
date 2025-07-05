#ifndef WINDOW_ENGINEWINDOW_H
#define WINDOW_ENGINEWINDOW_H

class EngineWindow {
public:
  virtual ~EngineWindow() = default;

  // Initialize the window
  virtual void initialize(int width, int height, const char *title) = 0;

  // Swap buffers
  virtual void swapBuffers() = 0;

  // Check if the window should close
  [[nodiscard]] virtual auto shouldClose() const -> bool = 0;

  [[nodiscard]] virtual auto getWidth() const -> int = 0;
  [[nodiscard]] virtual auto getHeight() const -> int = 0;
};

#endif // WINDOW_ENGINEWINDOW_H
