#include "ecs/DefaultWorld.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <print>
#include <utility>

DefaultWorld::DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept : registry(std::move(registry)) {}

auto DefaultWorld::createEntity() noexcept -> Entity {
  return nextEntity++;
}

auto DefaultWorld::copyEntity(Entity source) noexcept -> Entity {
  auto const entity = createEntity();
  registry->copyComponents(source, entity);
  return entity;
}

void DefaultWorld::removeEntity(Entity entity) noexcept {
  registry->removeAll(entity);
}

void DefaultWorld::registerSystem(std::shared_ptr<System> system) {
  std::println("Register system {}", system->id);
  systems[system->id] = system;
  sortedSystems.push_back(system);
  std::ranges::sort(
      sortedSystems,
      [](const auto &a, const auto &b)
      {
        return a->priority < b->priority;
      }
  );
  system->start();
  std::println("System {} started", system->id);
}

auto DefaultWorld::getSystem(SystemID systemId) const -> std::optional<std::shared_ptr<System>> {
  if (systems.contains(systemId)) {
    return systems.at(systemId);
  }
  return std::nullopt;
}

void DefaultWorld::update(float dt) {
  for (const auto &sys : sortedSystems) {
    sys->update(dt);
  }
}
