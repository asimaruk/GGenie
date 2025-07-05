#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string_view>

namespace graphics {
auto compileShader(std::string_view vertex, std::string_view fragment) -> unsigned int;
}

#endif
