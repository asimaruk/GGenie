#include "system/event/EventSystem.hpp"
#include <memory>

class GLFWEventsAdapter {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  GLFWEventsAdapter();
  ~GLFWEventsAdapter();
  void attach(std::shared_ptr<EventSystem> eventSystem);
  void detach(std::shared_ptr<EventSystem> eventSystem);
};
