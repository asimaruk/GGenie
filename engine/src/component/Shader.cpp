#include "component/Shader.h"
#include <string>
#include <string_view>
#include <utility>

Shader::Shader(std::string_view vertexSource, std::string_view fragmentSource) noexcept
    : vertexSource(vertexSource)
    , fragmentSource(fragmentSource) {}

Shader::Shader(std::string &&vertexSource, std::string &&fragmentSource) noexcept
    : vertexSource(std::move(vertexSource))
    , fragmentSource(std::move(fragmentSource)) {}
