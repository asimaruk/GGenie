#include "component/Transform.h"
#include "component/TransformTween.h"
#include "ecs/ComponentRegistry.hpp"
#include "math/algebras.h"
#include "utils/random.hpp"
#include <memory>
#include <numbers>

auto createInfiniteRotationTween(const Vec3 &translation, const Vec3 &scale, const Vec3 &axis, float duration) -> TransformTween {
  return TransformTween{
      .duration = duration,
      .from =
          Transform{
              .translation = translation,
              .rotation = Quat::fromAxisAngle(axis, std::numbers::pi * 0.001),
              .scale = scale,
          },
      .to =
          Transform{
              .translation = translation,
              .rotation = Quat::fromAxisAngle(axis, std::numbers::pi * 2),
              .scale = scale,
          },
      .time = 0.f,
      .repeat = TransformTween::INFINITY_REPEAT,
  };
}

void addInfiniteRandomRotationTween(std::shared_ptr<ComponentRegistry> registry, const Entity entity) {
  auto transformRef = registry->get<Transform>(entity);
  if (transformRef.has_value()) {
    static std::uniform_real_distribution<float> durationDistr(0.3, 30);
    auto transform = transformRef->get();
    auto tween = createInfiniteRotationTween(
        transform.translation,
        transform.scale,
        Vec3{
            .x = random_0_to_1<float>(),
            .y = random_0_to_1<float>(),
            .z = random_0_to_1<float>(),
        }.normalize(),
        randomDist(durationDistr)
    );
    registry->add(entity, tween);
  }
}