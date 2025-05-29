#include "component/Mesh.h"
#include <span>
#include <utility>
#include <vector>

Mesh::Mesh(std::vector<float> &&v, std::vector<unsigned int> &&i) noexcept
    : vertices(std::move(v))
    , indices(std::move(i)) {}

auto Mesh::cube() noexcept -> Mesh {
  return Mesh(
      // clang-format off
    // NOLINTBEGIN(readability-magic-numbers)
    {
    // Positions          // Colors
    -0.5F, -0.5F, -0.5F,  1.0F, 0.0F, 0.0F,
     0.5F, -0.5F, -0.5F,  0.0F, 1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  0.0F, 0.0F, 1.0F,
    -0.5F,  0.5F, -0.5F,  1.0F, 1.0F, 0.0F,
    -0.5F, -0.5F,  0.5F,  1.0F, 0.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  0.0F, 1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  0.5F, 0.5F, 0.5F,
    -0.5F,  0.5F,  0.5F,  1.0F, 1.0F, 1.0F
    },
    {
    0, 1, 2,  0, 2, 3,  // back face
    4, 5, 6,  4, 6, 7,  // forward face
    0, 4, 7,  0, 7, 3,  // left face
    1, 5, 6,  1, 6, 2,  // right face
    3, 2, 6,  3, 6, 7,  // top face
    0, 1, 5,  0, 5, 4   // bottom face
    } 
    // NOLINTEND(readability-magic-numbers)
        // clang-format on
  );
}

auto Mesh::getVertices() const noexcept -> std::span<const float> {
  return vertices;
}

auto Mesh::getIndices() const noexcept -> std::span<const unsigned int> {
  return indices;
}
