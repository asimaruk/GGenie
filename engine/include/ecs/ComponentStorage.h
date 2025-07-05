#ifndef ECS_COMPONENTSTORAGE_H
#define ECS_COMPONENTSTORAGE_H

#include "Entity.h"

class ComponentStorage {
public:
  virtual ~ComponentStorage() = default;
  virtual void remove(Entity e) noexcept = 0;
  virtual void copyComponent(Entity source, Entity target) noexcept = 0;
};

#endif
