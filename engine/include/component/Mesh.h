#pragma once

#include "ecs/Component.h"
#include <span>
#include <vector>

class Mesh final : public Component {
private:
  std::vector<float> vertices = {};
  std::vector<unsigned int> indices = {};

public:
  Mesh(std::vector<float> &&v, std::vector<unsigned int> &&i) noexcept;

  static Mesh cube() noexcept;

  virtual std::span<const float> getVertices() const noexcept final;
  virtual std::span<const unsigned int> getIndices() const noexcept final;
};