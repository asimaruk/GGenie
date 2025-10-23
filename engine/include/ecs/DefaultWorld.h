#ifndef ECS_DEFAULTWORLD_H
#define ECS_DEFAULTWORLD_H

#include "ComponentRegistry.hpp"
#include "Entity.h"
#include "System.h"
#include "World.h"

#include <optional>
#include <unordered_map>

namespace GGenie {

class DefaultWorld final : public World {
private:
  std::shared_ptr<ComponentRegistry> registry;
  Entity nextEntity = 0;
  std::unordered_map<SystemID, std::shared_ptr<System>> systems;
  std::vector<std::shared_ptr<System>> sortedSystems;

public:
  explicit DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept;

  auto createEntity() noexcept -> Entity override;
  auto copyEntity(Entity source) noexcept -> Entity override;
  void removeEntity(Entity entity) noexcept override;
  void registerSystem(std::shared_ptr<System> system) override;
  [[nodiscard]] auto getSystem(SystemID systemId) const -> std::optional<std::shared_ptr<System>> override;
  void update(float dt) override;
};

} // namespace GGenie

#endif
