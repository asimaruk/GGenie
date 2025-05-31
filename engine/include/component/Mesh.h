#pragma once

#include <vector>

struct Mesh {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  Mesh(std::vector<float> &&vertices, std::vector<unsigned int> &&indices) noexcept;

  static Mesh cube() noexcept;
};
