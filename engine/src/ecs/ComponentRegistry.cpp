#include "ecs/ComponentRegistry.h"
#include "ecs/DefaultComponentStorage.h"

template<typename T>
DefaultComponentStarage<T>* ComponentRegistry::getStorage() noexcept {
    if (storages.contains(typeid(T))) {
        return dynamic_cast<DefaultComponentStarage<T>>(storages[typeid(T)].get());
    }
    auto storage = DefaultComponentStarage<T>();
    storages[typeid(T)] = storage;
    return storage;
}

template<typename T>
void ComponentRegistry::add(Entity entity, const T& component) noexcept {
    getStorage<T>().add(entity, component);
}

template<typename T>
T* ComponentRegistry::get(Entity entity) noexcept {
    return getStorage<T>()->get();
}

template<typename T>
void ComponentRegistry::remove(Entity entity) noexcept {
    return getStorage<T>().remove(entity);
}

void ComponentRegistry::removeAll(Entity entity) noexcept {
    for (const auto& [_, storage] : storages) {
        storage->remove(entity);
    }
}