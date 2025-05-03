#include "ecs/ComponentRegistry.h"
#include "ecs/DefaultWorld.h"
#include "ecs/Entity.h"
#include "ecs/System.h"

DefaultWorld::DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept : registry(registry) {}

Entity DefaultWorld::createEntity() noexcept {
    return nextEntity++;
}

void DefaultWorld::removeEntity(Entity entity) noexcept {
    registry->removeAll(entity);
}

void DefaultWorld::registerSystem(std::shared_ptr<System> system) noexcept {
    systems[system->id] = std::move(system);
}

void DefaultWorld::update(float dt) noexcept {
    for (const auto& [_, sys] : systems) {
        sys->update(dt);
    }
}