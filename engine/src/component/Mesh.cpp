#include "component/Mesh.h"
#include <vector>

Mesh::Mesh(std::vector<float> &&v, std::vector<unsigned int> &&i) noexcept
    : vertices(std::move(v)), indices(std::move(i)) {}

std::span<const float> Mesh::getVertices() const noexcept { return vertices; }

std::span<const unsigned int> Mesh::getIndices() const noexcept {
  return indices;
}