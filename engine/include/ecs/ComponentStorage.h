#pragma once

#include "Entity.h"

class ComponentStorage {
public:
  virtual ~ComponentStorage() = default;
  virtual void remove(Entity e) noexcept = 0;
};