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
  void add(Entity entity, const T &component) noexcept;
  T *get(Entity entity) noexcept;
  void remove(Entity entity) noexcept override;
};
