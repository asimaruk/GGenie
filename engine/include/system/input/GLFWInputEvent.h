#pragma once

struct GLFWKeyboardEvent {
  int key;
  int scancode;
  int action;
  int mods;
};

struct GLFWMouseButtonEvent {
  int button;
  int action;
  int mods;
};

struct GLFWCursorPositionEvent {
  double xpos;
  double ypos;
};

struct GLFWScrollEvent {
  double xoffset;
  double yoffset;
};

struct GLFWCursorEnterEvent {
  int entered;
};
