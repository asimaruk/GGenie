#pragma once
#include "EngineWindow.h"

class GlfwEngineWindow: public EngineWindow {
private:
    GLFWwindow* window;
public:
    ~GlfwEngineWindow() override;
    void initialize(int width, int height, const char* title) override;
    void pollEvents() override;
    void swapBuffers() override;
    bool shouldClose() override;
};