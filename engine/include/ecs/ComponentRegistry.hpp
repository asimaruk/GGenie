#pragma once

#include "ComponentStorage.h"
#include "DefaultComponentStorage.hpp"
#include "Entity.h"
#include <typeindex>
#include <unordered_map>

class ComponentRegistry final {
private:
  std::unordered_map<std::type_index, std::shared_ptr<ComponentStorage>> storages;

  template <typename T> std::shared_ptr<DefaultComponentStorage<T>> getStorage() noexcept {
    auto it = storages.find(typeid(T));
    if (it != storages.end()) {
      return std::dynamic_pointer_cast<DefaultComponentStorage<T>>(it->second);
    }
    auto storage = std::make_shared<DefaultComponentStorage<T>>();
    storages[typeid(T)] = storage;
    return storage;
  }

public:
  template <typename T> void add(Entity e, const T &component) noexcept {
    getStorage<T>()->add(e, component);
  }

  template <typename T> void replace(Entity e, const T &component) noexcept {
    getStorage<T>()->replace(e, component);
  }

  template <typename T> std::optional<std::reference_wrapper<T>> get(Entity e) noexcept {
    return getStorage<T>()->get(e);
  }

  template <typename T> void remove(Entity e) noexcept {
    return getStorage<T>()->remove(e);
  }

  void removeAll(Entity e) noexcept {
    for (const auto &[_, storage] : storages) {
      storage->remove(e);
    }
  }

  template <typename T> EntityComponentRange<T> auto getAll() {
    return getStorage<T>()->all();
  }

  void copyComponents(Entity source, Entity target) noexcept {
    for (const auto &[type, storage] : storages) {
      storage->copyComponent(source, target);
    }
  }
};
