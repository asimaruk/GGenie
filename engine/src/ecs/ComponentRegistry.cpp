#include "ecs/ComponentRegistry.h"
#include "ecs/DefaultComponentStorage.h"
#include <component/CubeMesh.h>
#include <component/Shader.h>
#include <component/Transform.h>
#include <memory>

template <typename T> std::shared_ptr<DefaultComponentStorage<T>> ComponentRegistry::getStorage() noexcept {
  auto it = storages.find(typeid(T));
  if (it != storages.end()) {
    return std::dynamic_pointer_cast<DefaultComponentStorage<T>>(it->second);
  }
  auto storage = std::make_shared<DefaultComponentStorage<T>>();
  storages[typeid(T)] = storage;
  return storage;
}

template <typename T> void ComponentRegistry::add(Entity entity, const T &component) noexcept {
  getStorage<T>()->add(entity, component);
}

// Don't like it. Think of moving add() declaration to hpp file
template void ComponentRegistry::add<CubeMesh>(Entity, const CubeMesh &);
template void ComponentRegistry::add<Shader>(Entity, const Shader &);
template void ComponentRegistry::add<Transform>(Entity, const Transform &);

template <typename T> T *ComponentRegistry::get(Entity entity) noexcept { return getStorage<T>()->get(entity); }

template CubeMesh *ComponentRegistry::get<CubeMesh>(Entity);
template Shader *ComponentRegistry::get<Shader>(Entity);
template Transform *ComponentRegistry::get<Transform>(Entity);

template <typename T> void ComponentRegistry::remove(Entity entity) noexcept { return getStorage<T>()->remove(entity); }

template void ComponentRegistry::remove<CubeMesh>(Entity);
template void ComponentRegistry::remove<Shader>(Entity);
template void ComponentRegistry::remove<Transform>(Entity);

void ComponentRegistry::removeAll(Entity entity) noexcept {
  for (const auto &[_, storage] : storages) {
    storage->remove(entity);
  }
}