#pragma once

#include "ecs/Component.h"
#include <string>

class Shader final : public Component {
private:
  std::string vertexSource;
  std::string fragmentSource;

public:
  Shader(std::string_view vs, std::string_view fs) noexcept;
  Shader(std::string &&vs, std::string &&fs) noexcept;
  std::string_view getVertex() const;
  std::string_view getFragment() const;
};