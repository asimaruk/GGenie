#pragma once

#include "Component.h"
#include "ComponentRegistry.h"
#include "Entity.h"
#include "System.h"
#include "World.h"

#include <typeindex>
#include <unordered_map>
#include <vector>

class DefaultWorld final : public World {
private:
    std::shared_ptr<ComponentRegistry> registry;
    Entity nextEntity = 0;
    std::unordered_map<SystemID, std::shared_ptr<System>> systems;
public:
    DefaultWorld(std::shared_ptr<ComponentRegistry> registry) noexcept;

    Entity createEntity() noexcept override;
    void removeEntity(const Entity entity) noexcept override;
    void registerSystem(std::shared_ptr<System> system) noexcept override;
    void update(float dt) noexcept override;
};
