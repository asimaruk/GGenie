#pragma once

#include "ecs/Component.h"
#include <string>

class Shader final : public Component {
private:
  std::string vertexSource;
  std::string fragmentSource;

public:
  Shader(std::string_view vertexSource, std::string_view fragmentSource) noexcept;
  Shader(std::string &&vertexSource, std::string &&fragmentSource) noexcept;
  std::string_view getVertex() const;
  std::string_view getFragment() const;
};