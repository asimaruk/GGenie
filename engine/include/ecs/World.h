#pragma once

#include "Entity.h"
#include "System.h"
#include <memory>

class World {
public:
  virtual ~World();

  virtual Entity createEntity() noexcept = 0;
  virtual void removeEntity(const Entity entity) noexcept = 0;
  virtual void registerSystem(const std::shared_ptr<System> system) noexcept = 0;
  virtual void update(float dt) noexcept = 0;
};
