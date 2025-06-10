#pragma once

#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "system/event/Event.h"
#include "system/event/EventSystem.hpp"
#include <memory>

class FirstPersonViewControlSystem : public System {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  static constexpr auto ID = "FirstPersonViewControlSystemID";
  FirstPersonViewControlSystem(
      SystemID systemId,
      int priority,
      std::shared_ptr<ComponentRegistry> registry,
      std::shared_ptr<EventSystem> eventSystem
  );
  ~FirstPersonViewControlSystem();
  void start() override;
  void update(float dt) override;
  void setControlledEntity(Entity entity) noexcept;
};
