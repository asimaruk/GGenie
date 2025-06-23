#pragma once

#include <string_view>

namespace graphics {
unsigned int compileShader(std::string_view vertex, std::string_view fragment);
}