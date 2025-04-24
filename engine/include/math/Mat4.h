#pragma once

#include <array>
#include "Vec3.h"
#include "Quat.h"

class Mat4 final {
private:
    std::array<float, 16> data;
public:
    explicit Mat4(std::array<float, 16> data) noexcept;
    explicit Mat4(float fill) noexcept;

    constexpr static Mat4 identity() noexcept;
    static Mat4 translation(const Vec3& v) noexcept;
    static Mat4 rotation(const Quat& q) noexcept;
    static Mat4 scale(const Vec3& v) noexcept;

    Mat4 operator*(const Mat4& other) const noexcept;
    
    const float* getData() const noexcept;
};