#include "component/CubeMesh.h"
#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/DefaultComponentStorage.h"
#include "ecs/Entity.h"
#include <vector>
#include <unordered_map>

template<typename T>
void DefaultComponentStorage<T>::add(Entity entity, const T& component) noexcept {
    size_t index = components.size();
    entity2index[entity] = index;
    index2entity.push_back(entity);
    components.push_back(component);
}

template void DefaultComponentStorage<CubeMesh>::add(Entity, const CubeMesh&);
template void DefaultComponentStorage<Shader>::add(Entity, const Shader&);
template void DefaultComponentStorage<Transform>::add(Entity, const Transform&);

template<typename T>
T* DefaultComponentStorage<T>::get(Entity entity) noexcept {
    auto it = entity2index.find(entity);
    if (it == entity2index.end()) {
        return nullptr;
    }
    return &components[it->second];
}

template CubeMesh* DefaultComponentStorage<CubeMesh>::get(Entity);
template Shader* DefaultComponentStorage<Shader>::get(Entity);
template Transform* DefaultComponentStorage<Transform>::get(Entity);

template<typename T>
void DefaultComponentStorage<T>::remove(Entity entity) noexcept {
    auto it = entity2index.find(entity);
    if (it == entity2index.end()) {
        return;
    }

    size_t index = it->second;
    size_t lastIndex = components.size() - 1;

    if (index != lastIndex) {
        components[index] = std::move(components.back());
        auto lastEntity = index2entity[lastIndex];
        entity2index[lastEntity] = index;
        index2entity[index] = lastEntity;
    }

    components.pop_back();
    index2entity.pop_back();
    entity2index.erase(entity);
}

template void DefaultComponentStorage<CubeMesh>::remove(Entity);
template void DefaultComponentStorage<Shader>::remove(Entity);
template void DefaultComponentStorage<Transform>::remove(Entity);
