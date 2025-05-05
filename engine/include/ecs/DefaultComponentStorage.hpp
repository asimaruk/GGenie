#pragma once

#include "ComponentStorage.h"
#include <unordered_map>
#include <vector>

template <typename T> class DefaultComponentStorage : public ComponentStorage {
private:
  std::vector<T> components;
  std::unordered_map<Entity, size_t> entity2index;
  std::vector<Entity> index2entity;

public:
  void add(Entity entity, const T &component) noexcept {
    size_t index = components.size();
    entity2index[entity] = index;
    index2entity.push_back(entity);
    components.push_back(component);
  }

  T *get(Entity entity) noexcept {
    auto it = entity2index.find(entity);
    if (it == entity2index.end()) {
      return nullptr;
    }
    return &components[it->second];
  }

  void remove(Entity entity) noexcept override {
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
};
