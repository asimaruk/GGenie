#pragma once

#include <string>

class Shader final {
private:
    std::string vertexSource;
    std::string fragmentSource;
public:
    Shader(std::string_view vs, std::string_view fs) noexcept;
    Shader(std::string&& vs, std::string&& fs) noexcept;

    // запрет копирования
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // разрешение перемещения
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    std::string_view getVertex();
    std::string_view getFragment();
};