#ifndef ECS_WORLD_H
#define ECS_WORLD_H

#include "Entity.h"
#include "System.h"
#include <memory>
#include <optional>

class World {
public:
  virtual ~World();

  virtual auto createEntity() noexcept -> Entity = 0;
  virtual auto copyEntity(Entity source) noexcept -> Entity = 0;
  virtual void removeEntity(const Entity entity) noexcept = 0;
  virtual void registerSystem(const std::shared_ptr<System> system) = 0;
  [[nodiscard]] virtual auto getSystem(SystemID id) const -> std::optional<std::shared_ptr<System>> = 0;
  virtual void update(float dt) = 0;
};

#endif
