#pragma once

#include "ComponentStorage.h"
#include "DefaultComponentStorage.h"
#include "Entity.h"

#include <unordered_map>
#include <typeindex>

class ComponentRegistry {
private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentStorage>> storages;

    template<typename T>
    DefaultComponentStarage<T>* getStorage() noexcept;
public:
    template<typename T>
    void add(Entity e, const T& component) noexcept;
    template<typename T>
    T* get(Entity e) noexcept;
    template<typename T>
    void remove(Entity e) noexcept;
    void removeAll(Entity e) noexcept;
};
