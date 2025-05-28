#include "component/Shader.h"
#include <string>

Shader::Shader(std::string_view vs, std::string_view fs) noexcept : vertexSource(vs), fragmentSource(fs) {}

Shader::Shader(std::string &&vs, std::string &&fs) noexcept
    : vertexSource(std::move(vs))
    , fragmentSource(std::move(fs)) {}

std::string_view Shader::getVertex() const {
  return vertexSource;
}

std::string_view Shader::getFragment() const {
  return fragmentSource;
}
