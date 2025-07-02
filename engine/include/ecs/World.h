#pragma once

#include "Entity.h"
#include "System.h"
#include <memory>
#include <optional>

class World {
public:
  virtual ~World();

  virtual Entity createEntity() noexcept = 0;
  virtual Entity copyEntity(Entity source) noexcept = 0;
  virtual void removeEntity(const Entity entity) noexcept = 0;
  virtual void registerSystem(const std::shared_ptr<System> system) = 0;
  virtual std::optional<std::shared_ptr<System>> getSystem(SystemID id) const = 0;
  virtual void update(float dt) = 0;
};
