#include "ecs/DefaultWorld.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <__ostream/print.h>
#include <algorithm>

DefaultWorld::DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept : registry(registry) {}

Entity DefaultWorld::createEntity() noexcept { return nextEntity++; }

void DefaultWorld::removeEntity(Entity entity) noexcept { registry->removeAll(entity); }

void DefaultWorld::registerSystem(std::shared_ptr<System> system) noexcept {
  std::println("Register system {}", system->id);
  systems[system->id] = system;
  sortedSystems.push_back(system);
  std::sort(sortedSystems.begin(), sortedSystems.end(),
            [](const auto &a, const auto &b) { return a->priority < b->priority; });
  system->start();
  std::println("System {} started", system->id);
}

std::optional<std::shared_ptr<System>> DefaultWorld::getSystem(SystemID systemId) const {
  if (systems.contains(systemId)) {
    return systems.at(systemId);
  }
  return std::nullopt;
}

void DefaultWorld::update(float dt) noexcept {
  for (const auto &sys : sortedSystems) {
    sys->update(dt);
  }
}
