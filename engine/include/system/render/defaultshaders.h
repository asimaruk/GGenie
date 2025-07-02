#pragma once

#include <component/Shader.h>
#include <string_view>
#include <vector>

namespace shaders {

inline constexpr std::string_view DEFAULT_VERTEX = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    
    out vec3 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

inline constexpr std::string_view DEFAULT_FRAGMENT = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";

const Shader::Attributes DEFAULT_ATTRS[2] = {
    {0, 3, Shader::VertexType::FLOAT, false, 6 * sizeof(float), (void *)nullptr},
    {1, 3, Shader::VertexType::FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float))}
};
} // namespace shaders
