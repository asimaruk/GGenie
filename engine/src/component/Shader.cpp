#include "component/Shader.h"
#include <span>
#include <string_view>

Shader::Shader(
    std::string_view vertexSource, std::string_view fragmentSource, std::span<const Attributes> attrs
) noexcept
    : vertexSource(vertexSource)
    , fragmentSource(fragmentSource)
    , attrs(attrs) {}
