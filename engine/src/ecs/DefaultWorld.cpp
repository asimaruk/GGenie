#include "ecs/DefaultWorld.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <__ostream/print.h>

DefaultWorld::DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept : registry(registry) {}

Entity DefaultWorld::createEntity() noexcept { return nextEntity++; }

void DefaultWorld::removeEntity(Entity entity) noexcept { registry->removeAll(entity); }

void DefaultWorld::registerSystem(std::shared_ptr<System> system) noexcept { 
  std::println("Register system {}", system->id);
  systems[system->id] = system;
  system->start();
  std::println("System {} started", system->id);
}

void DefaultWorld::update(float dt) noexcept {
  for (const auto &[_, sys] : systems) {
    sys->update(dt);
  }
}