#include "math/Vec3.h"
#include <cmath>       // for std::sqrt
#include <stdexcept>   // for std::out_of_range

// Initialize vector with components
Vec3::Vec3(float x, float y, float z) noexcept : data{x, y, z} {}

const Vec3 Vec3::ONE = Vec3(1, 1, 1);

Vec3 Vec3::fill(float value) noexcept {
    return Vec3(value, value, value);
}

// Component getters
float Vec3::x() const noexcept { return data[0]; }  // x is at index 0
float Vec3::y() const noexcept { return data[1]; }  // y is at index 1
float Vec3::z() const noexcept { return data[2]; }  // z is at index 2

// Safe indexed access (throws if index >= 3)
float Vec3::get(size_t index) const {
    if (index >= 3) throw std::out_of_range("Vec3 index out of range");
    return data[index];
}

// Computes ||v|| = sqrt(x² + y² + z²)
float Vec3::length() const {
    return std::sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
}

// Structured binding support - compile-time checked access
template<size_t N>
float get(const Vec3& v) {
    static_assert(N < 3, "Vec3 has only 3 components");
    return v.get(N);
}

// Explicit template instantiations (required for linking)
template float get<0>(const Vec3&);  // x-component
template float get<1>(const Vec3&);  // y-component
template float get<2>(const Vec3&);  // z-component

Vec3 Vec3::operator+(const Vec3& other) const noexcept {
    return Vec3(x() + other.x(), y() + other.y(), z() + other.z());
}