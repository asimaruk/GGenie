#include "component/Material.h"
#include "component/Mesh.h"
#include "component/Transform.h"
#include "component/TransformTween.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/World.h"
#include "math/algebras.h"
#include "system/render/defaultshaders.h"
#include "utils/random.hpp"
#include <memory>
#include <numbers>
#include <print>

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

void setupCubes(std::shared_ptr<World> world, std::shared_ptr<ComponentRegistry> registry) {
  auto cube = world->createEntity();
  auto mesh = Mesh::cube();
  auto material = Material(shaders::DEFAULT_VERTEX, shaders::DEFAULT_FRAGMENT, shaders::VERTEX_COLOR_ATTRIBUTES);

  static auto minPos = -25.f;
  static auto maxPos = 25.f;
  static auto len = maxPos - minPos;
  static auto size = 10;
  static auto doubleSize = size * size;
  static auto tripleSize = size * size * size;
  for (int i = 0; i < tripleSize; ++i) {
    auto cube = world->createEntity();
    registry->add(cube, mesh);
    registry->add(cube, material);
    auto xStep = i % size;
    auto yStep = std::floor(static_cast<float>(i % doubleSize) / size);
    auto zStep = std::floor(i / static_cast<float>(doubleSize));
    auto x = minPos + (xStep / static_cast<float>(size)) * len;
    auto y = minPos + (yStep / size) * len;
    auto z = minPos + (zStep / size) * len;
    registry->add(
        cube,
        Transform{.translation{
            .x = x,
            .y = y,
            .z = z,
        }}
    );
    if (static_cast<int>(xStep + yStep + zStep) % 2 == 1) {
      addInfiniteRandomRotationTween(registry, cube);
    }
  }
}
