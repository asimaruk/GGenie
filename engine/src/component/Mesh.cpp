#include "component/Mesh.h"
#include <vector>

Mesh::Mesh(std::vector<float> &&v, std::vector<unsigned int> &&i) noexcept
    : vertices(std::move(v))
    , indices(std::move(i)) {}

Mesh Mesh::cube() noexcept {
  return Mesh(
      // clang-format off
    {
    // Позиции                        // Цвета
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f
    },
    {
    0, 1, 2,  0, 2, 3,  // задняя грань
    4, 5, 6,  4, 6, 7,  // передняя грань
    0, 4, 7,  0, 7, 3,  // левая грань
    1, 5, 6,  1, 6, 2,  // правая грань
    3, 2, 6,  3, 6, 7,  // верхняя грань
    0, 1, 5,  0, 5, 4   // нижняя грань
    } // clang-format on
  );
}

std::span<const float> Mesh::getVertices() const noexcept {
  return vertices;
}

std::span<const unsigned int> Mesh::getIndices() const noexcept {
  return indices;
}
