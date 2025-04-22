#include "component/Transform.h"
#include "math/Mat4.h"
#include "math/Quat.h"

Transform::Transform() noexcept 
: translation(0, 0, 0),
  rotation(Quat::identity()),
  scale(1, 1, 1),
  isDirty(false),
  matrix(Mat4::identity())
{}

void Transform::setTranslation(const Vec3& v) noexcept {
    translation = v;
    isDirty = true;
}
    
void Transform::setScale(const Vec3& v) noexcept {
    scale = v;
    isDirty = true;
}
    
void Transform::setRotatation(const float radians, const Vec3& axis) noexcept {
    rotation = Quat::fromAxisAngle(axis, radians);
    isDirty = true;
}
    
Mat4 Transform::getMatrix() noexcept {
    if (isDirty) {
        isDirty = false;
        matrix = Mat4::translation(translation) * Mat4::rotation(rotation) * Mat4::scale(scale);
    }
    return matrix;
}