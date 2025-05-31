#include "component/Mesh.h"
#include <utility>
#include <vector>

Mesh::Mesh(std::vector<float> &&vertices, std::vector<unsigned int> &&indices) noexcept
    : vertices(std::forward<std::vector<float>>(vertices))
    , indices(std::forward<std::vector<unsigned int>>(indices)) {}

auto Mesh::cube() noexcept -> Mesh {
  // clang-format off
  // NOLINTBEGIN(readability-magic-numbers)
  return Mesh(
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
  );
  // NOLINTEND(readability-magic-numbers)
  // clang-format on
}
