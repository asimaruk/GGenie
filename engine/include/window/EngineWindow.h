#ifndef WINDOW_H
#define WINDOW_H

class EngineWindow {
public:
  virtual ~EngineWindow() = default;

  // Initialize the window
  virtual void initialize(int width, int height, const char *title) = 0;

  // Poll for events
  virtual void pollEvents() = 0;

  // Swap buffers
  virtual void swapBuffers() = 0;

  // Check if the window should close
  virtual bool shouldClose() = 0;
};

#endif // WINDOW_H