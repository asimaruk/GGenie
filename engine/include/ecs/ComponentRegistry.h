#pragma once

#include "ComponentStorage.h"
#include "DefaultComponentStorage.h"
#include "Entity.h"
#include <typeindex>
#include <unordered_map>

class ComponentRegistry final {
private:
  std::unordered_map<std::type_index, std::shared_ptr<ComponentStorage>> storages;

  template <typename T> std::shared_ptr<DefaultComponentStorage<T>> getStorage() noexcept;

public:
  template <typename T> void add(Entity e, const T &component) noexcept;
  template <typename T> T *get(Entity e) noexcept;
  template <typename T> void remove(Entity e) noexcept;
  void removeAll(Entity e) noexcept;
};
