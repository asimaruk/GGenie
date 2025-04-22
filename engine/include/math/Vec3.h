#pragma once

#include <array>

class Vec3 final {
private:
    std::array<float, 3> data;  // coordinates (x, y, z)

public:
    Vec3(float x, float y, float z) noexcept;

    // Index-based access with bounds checking
    float get(size_t i) const;

    // Read-only accessors (no setters to enforce immutability)
    float x() const noexcept;
    float y() const noexcept;
    float z() const noexcept;

    float length() const;

    static const Vec3 ONE;

    static Vec3 fill(float value) noexcept;

    Vec3 operator+(const Vec3& other) const noexcept;
};

namespace std {
    // Vec3 has 3 elements
    template<>
    struct tuple_size<Vec3> : integral_constant<size_t, 3> {};

    // Each element is float
    template<size_t N>
    struct tuple_element<N, Vec3> { using type = float; };
}

// Функция для доступа к элементам (нужна для structured binding)
template<size_t N>
float get(const Vec3& v);