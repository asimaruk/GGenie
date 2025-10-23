#ifndef COMPONENT_MESH_H
#define COMPONENT_MESH_H

#include <initializer_list>
#include <vector>

namespace GGenie {

struct Mesh {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  Mesh(std::vector<float> verts, std::vector<unsigned int> idxs) noexcept;
  Mesh(std::initializer_list<float> verts, std::initializer_list<unsigned int> idxs);

  static auto cube() -> Mesh;
};

} // namespace GGenie

#endif
