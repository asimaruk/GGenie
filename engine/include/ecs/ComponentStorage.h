#pragma once

#include "Entity.h"
#include "Component.h"

class ComponentStorage {
public:
    virtual ~ComponentStorage() = default;
    virtual void remove(Entity e) noexcept = 0;
};