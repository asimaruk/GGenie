#pragma once

#include <string>

struct Shader {
  std::string vertexSource;
  std::string fragmentSource;

  Shader(std::string_view vertexSource, std::string_view fragmentSource) noexcept;
  Shader(std::string &&vertexSource, std::string &&fragmentSource) noexcept;
};
