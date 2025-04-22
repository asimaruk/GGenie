#pragma once

#include <array>
#include "Vec3.h"

class Quat final {
private:
    std::array<float, 4> data;  // quaternion components (w, x, y, z)
    Quat(float w, float x, float y, float z) noexcept;
    Quat(const Vec3& axis, float radians) noexcept;
    Quat(float pitch, float yaw, float roll) noexcept;
public:
    static Quat identity() noexcept;
    static Quat fromAxisAngle(const Vec3& axis, float radians) noexcept;
    static Quat fromEuler(float pitch, float yaw, float roll) noexcept;

    // Index-based access with bounds checking
    float get(size_t i) const;

    // Read-only accessors (no setters to enforce immutability)
    float w() const noexcept;
    float x() const noexcept;
    float y() const noexcept;
    float z() const noexcept;

    Quat normalized() const;
};

namespace std {
    // Quat has 4 elements
    template<>
    struct tuple_size<Quat> : integral_constant<size_t, 4> {};

    // Each element is float
    template<size_t N>
    struct tuple_element<N, Quat> { using type = float; };
}

template<size_t N>
float get(const Quat& v);