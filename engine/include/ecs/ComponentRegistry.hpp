#ifndef ECS_COMPONENTREGISTRY_HPP
#define ECS_COMPONENTREGISTRY_HPP

#include "ComponentStorage.h"
#include "DefaultComponentStorage.hpp"
#include "Entity.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

class ComponentRegistry final {
private:
  std::unordered_map<std::type_index, std::shared_ptr<ComponentStorage>> storages;

  template <typename T> auto getStorage() -> std::shared_ptr<DefaultComponentStorage<T>> {
    auto iter = storages.find(typeid(T));
    if (iter != storages.end()) {
      return std::dynamic_pointer_cast<DefaultComponentStorage<T>>(iter->second);
    }
    auto storage = std::make_shared<DefaultComponentStorage<T>>();
    storages[typeid(T)] = storage;
    return storage;
  }

public:
  template <typename T> void add(Entity entity, const T &component) noexcept {
    getStorage<T>()->add(entity, component);
  }

  template <typename T> void replace(Entity entity, const T &component) {
    getStorage<T>()->replace(entity, component);
  }

  template <typename T> auto get(Entity entity) noexcept -> std::optional<std::reference_wrapper<T>> {
    return getStorage<T>()->get(entity);
  }

  template <typename T> void remove(Entity entity) noexcept {
    return getStorage<T>()->remove(entity);
  }

  void removeAll(Entity entity) noexcept {
    for (const auto &[/*unused*/ _, storage] : storages) {
      storage->remove(entity);
    }
  }

  template <typename T> auto getAll() -> EntityComponentRange<T> auto {
    return getStorage<T>()->all();
  }

  void copyComponents(Entity source, Entity target) noexcept {
    for (const auto &[type, storage] : storages) {
      storage->copyComponent(source, target);
    }
  }
};

#endif
