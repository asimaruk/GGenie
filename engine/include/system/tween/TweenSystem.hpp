#pragma once

#include "Interpolator.hpp"
#include "Tween.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <memory>
#include <utility>
#include <vector>

class TweenSystem : public System {
private:
  std::shared_ptr<ComponentRegistry> registry;
  std::vector<std::pair<Entity, std::unique_ptr<TweenBase>>> tweens;

  template <Lerpable T> struct TweenWrapper : public TweenBase {
    const TweenSystem &system;
    const Entity entity;
    std::shared_ptr<Tween<T>> tween;

    TweenWrapper(const TweenSystem &system, Entity entity,
                 std::shared_ptr<Tween<T>> tween)
        : system(system), entity(entity), tween(std::move(tween)) {}

    void update(float dt) override {
      tween->update(dt);
      system.registry->replace(entity, tween->getValue());
    }

    bool isComplete() const override { return tween->isComplete(); }
  };

public:
  static constexpr auto ID = "TweenSystemID";

  TweenSystem(SystemID id, int priority,
              std::shared_ptr<ComponentRegistry> registry) noexcept
      : System(id, priority), registry(registry) {};

  void start() override {}

  void update(float dt) override {
    for (auto it = tweens.begin(); it != tweens.end();) {
      it->second->update(dt);
      if (it->second->isComplete()) {
        it = tweens.erase(it);
      } else {
        ++it;
      }
    }
  }

  template <Lerpable T>
  void tween(Entity entity, std::shared_ptr<Tween<T>> tween) {
    tweens.push_back(
        {entity, std::make_unique<TweenWrapper<T>>(*this, entity, tween)});
  }
};
