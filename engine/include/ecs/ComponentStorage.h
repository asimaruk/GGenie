#ifndef ECS_COMPONENTSTORAGE_H
#define ECS_COMPONENTSTORAGE_H

#include "Entity.h"

namespace GGenie {

class ComponentStorage {
public:
  ComponentStorage() = default;
  ComponentStorage(const ComponentStorage &) = delete;
  ComponentStorage(ComponentStorage &&) = delete;
  virtual ~ComponentStorage() = default;

  auto operator=(const ComponentStorage &) -> ComponentStorage & = delete;
  auto operator=(ComponentStorage &&) -> ComponentStorage & = delete;

  virtual void remove(Entity entity) noexcept = 0;
  virtual void copyComponent(Entity source, Entity target) = 0;
};

} // namespace GGenie

#endif
