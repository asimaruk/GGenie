#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include <span>
#include <string_view>

namespace GGenie {

struct Material {
  enum VertexType : std::int8_t {
    BYTE,
    UNSIGNED_BYTE,
    SHORT,
    UNSIGNED_SHORT,
    INT,
    UNSIGNED_INT,
    FLOAT,
  };

  struct Attributes {
    int index;
    int size;
    VertexType type;
    bool normalized;
    int stride;
    void *pointer;
  };

  std::string_view vertexSource;
  std::string_view fragmentSource;
  std::span<const Attributes> attrs;

  Material(std::string_view vertexSource, std::string_view fragmentSource, std::span<const Attributes> attrs) noexcept;
};

} // namespace GGenie

#endif
