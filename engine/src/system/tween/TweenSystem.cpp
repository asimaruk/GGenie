#include "component/TransformTween.h"
#include "system/tween/TweenSystem.h"
#include "ecs/ComponentRegistry.hpp"
#include "system/tween/Interpolator.hpp"
#include <algorithm>
#include <memory>
#include <utility>

class TweenSystem::Impl {
private:
  std::shared_ptr<ComponentRegistry> registry;

  void updateTransformTweens(float dt) {
    for (const auto [entity, tweenRef]: registry->getAll<TransformTween>()) {
      auto tween = tweenRef.get();
      TransformTween updateTween {
        .duration = tween.duration,
        .from = tween.from,
        .to = tween.to,
        .time = tween.time + dt,
      };
      const auto t = std::min(1.f, updateTween.time / updateTween.duration);
      const auto tweenedTransform = Interpolator::LinearInterpolator(updateTween.from, updateTween.to, t);
      registry->replace(entity, tweenedTransform);
      if (updateTween.time < updateTween.duration) {
        registry->replace(entity, updateTween);
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
