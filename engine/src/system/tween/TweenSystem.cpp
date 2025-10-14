#include "system/tween/TweenSystem.h"
#include "component/TransformTween.h"
#include "ecs/ComponentRegistry.hpp"
#include "system/tween/Interpolator.hpp"
#include <algorithm>
#include <memory>
#include <utility>

class TweenSystem::Impl {
private:
  std::shared_ptr<ComponentRegistry> registry;

  void updateTransformTweens(float dt) {
    for (const auto [entity, tweenRef] : registry->getAll<TransformTween>()) {
      auto tween = tweenRef.get();
      if (tween.repeat == 0) {
        registry->remove<TransformTween>(entity);
        continue;
      }
      auto updateTime = tween.time + dt;
      auto repeatRound = std::floor(updateTime / tween.duration);
      const auto timeNorm = (updateTime / tween.duration) - repeatRound;
      const auto tweenedTransform = Interpolator::LinearInterpolator(tween.from, tween.to, timeNorm);
      registry->replace(entity, tweenedTransform);
      if (repeatRound < tween.repeat || tween.repeat == TransformTween::INFINITY_REPEAT) {
        registry->replace(
            entity,
            TransformTween{
                .duration = tween.duration,
                .from = tween.from,
                .to = tween.to,
                .time = updateTime,
                .repeat = tween.repeat,
            }
        );
      } else {
        registry->remove<TransformTween>(entity);
      }
    }
  }

public:
  Impl(const std::shared_ptr<ComponentRegistry> registry) : registry(std::move(registry)) {}
  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;
  ~Impl() = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  void update(float dt) {
    updateTransformTweens(dt);
  }
};

TweenSystem::TweenSystem(const SystemID id, const int priority, std::shared_ptr<ComponentRegistry> registry) noexcept
    : System(id, priority)
    , pimpl(new Impl(registry)) {}

TweenSystem::~TweenSystem() = default;

void TweenSystem::start() {}

void TweenSystem::update(float dt) {
  pimpl->update(dt);
}
