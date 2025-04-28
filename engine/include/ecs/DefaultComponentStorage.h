#pragma once

#include "ComponentStorage.h"
#include <vector>
#include <unordered_map>

template<typename T>
class DefaultComponentStarage: public ComponentStorage {
private:
    std::vector<T> components;
    std::unordered_map<Entity, size_t> entity_to_index;
    std::vector<Entity> index_to_entity;
public:
    void add(Entity entity, const T& component) noexcept;
    T* get(Entity entity) noexcept;
    void remove(Entity entity) noexcept override;
};
