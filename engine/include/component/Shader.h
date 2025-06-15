#pragma once

#include <string>
#include <vector>

struct Shader {
  enum VertexType {
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

  std::string vertexSource;
  std::string fragmentSource;
  std::vector<Attributes> attrs;

  Shader(std::string_view vertexSource, std::string_view fragmentSource, std::vector<Attributes> &&attrs) noexcept;
  Shader(std::string &&vertexSource, std::string &&fragmentSource, std::vector<Attributes> &&attrs) noexcept;
};
