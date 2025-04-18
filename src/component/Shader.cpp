#include <string>

class Shader {
private:
    std::string vertexSource;
    std::string fragmentSource;
public:
    Shader(std::string_view vs, std::string_view fs) noexcept
        : vertexSource(vs), 
          fragmentSource(fs) {}

    Shader(std::string&& vs, std::string&& fs) noexcept
        : vertexSource(std::move(vs)), 
          fragmentSource(std::move(fs)) {}

    // запрет копирования
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // разрешение перемещения
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    std::string_view getVertex() {
        return vertexSource;
    }

    std::string_view getFragment() {
        return fragmentSource;
    }
};