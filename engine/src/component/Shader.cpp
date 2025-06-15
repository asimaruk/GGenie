#include "component/Shader.h"
#include <string>
#include <string_view>
#include <utility>
#include <vector>

Shader::Shader(std::string_view vertexSource, std::string_view fragmentSource, std::vector<Attributes> &&attrs) noexcept
    : vertexSource(vertexSource)
    , fragmentSource(fragmentSource)
    , attrs(std::move(attrs)) {}

Shader::Shader(std::string &&vertexSource, std::string &&fragmentSource, std::vector<Attributes> &&attrs) noexcept
    : vertexSource(std::move(vertexSource))
    , fragmentSource(std::move(fragmentSource))
    , attrs(std::move(attrs)) {}
