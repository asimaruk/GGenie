#pragma once

#include "ComponentRegistry.hpp"
#include "Entity.h"
#include "System.h"
#include "World.h"

#include <optional>
#include <unordered_map>

class DefaultWorld final : public World {
private:
  std::shared_ptr<ComponentRegistry> registry;
  Entity nextEntity = 0;
  std::unordered_map<SystemID, std::shared_ptr<System>> systems;
  std::vector<std::shared_ptr<System>> sortedSystems;

public:
  DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept;

  Entity createEntity() noexcept override;
  Entity copyEntity(Entity source) noexcept override;
  void removeEntity(const Entity entity) noexcept override;
  void registerSystem(std::shared_ptr<System> system) override;
  std::optional<std::shared_ptr<System>> getSystem(SystemID systemId) const override;
  void update(float dt) override;
};
