#pragma once

#include "Entity.h"

class ComponentStorage {
public:
  virtual ~ComponentStorage() = default;
  virtual void remove(Entity e) noexcept = 0;
  virtual void copyComponent(Entity source, Entity target) noexcept = 0;
};