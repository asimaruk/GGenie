#pragma once

#include <component/Material.h>
#include <string_view>
#include <vector>

namespace shaders {

inline constexpr std::string_view DEFAULT_VERTEX = "shader/default-vertex.glsl";
inline constexpr std::string_view DEFAULT_FRAGMENT = "shader/default-fragment.glsl";

const Material::Attributes VERTEX_COLOR_ATTRIBUTES[2] = {
    {0, 3, Material::VertexType::FLOAT, false, 6 * sizeof(float), (void *)nullptr},
    {1, 3, Material::VertexType::FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float))}
};
} // namespace shaders
