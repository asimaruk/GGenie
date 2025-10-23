#include "component/Material.h"
#include <span>
#include <string_view>

Material::Material(
    std::string_view vertexSource, std::string_view fragmentSource, std::span<const Attributes> attrs
) noexcept
    : vertexSource(vertexSource)
    , fragmentSource(fragmentSource)
    , attrs(attrs) {}
