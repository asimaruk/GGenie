#pragma once

#include "math/Vec3.h"
#include "math/Quat.h"
#include "math/Mat4.h"

class Transform final {
private:
    Vec3 translation;
    Quat rotation;
    Vec3 scale;
    bool isDirty;
    Mat4 matrix;
public:
    Transform() noexcept;
    void setTranslation(const Vec3& v) noexcept;
    void setScale(const Vec3& v) noexcept;
    void setRotatation(const float radians, const Vec3& axis) noexcept;
    Mat4 getMatrix() noexcept;
};