#ifndef COMPONENT_SHADER_H
#define COMPONENT_SHADER_H

#include <span>
#include <string_view>

struct Shader {
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

  Shader(std::string_view vertexSource, std::string_view fragmentSource, std::span<const Attributes> attrs) noexcept;
};

#endif
