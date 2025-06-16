#pragma once

#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "Tween.hpp"
#include <memory>
#include <utility>
#include <vector>

class TweenSystem : public System {
private:
  std::shared_ptr<ComponentRegistry> registry;
  // use unique_ptr as a pair second to store base non-generic abstract class for tweens
  std::vector<std::pair<Entity, std::unique_ptr<TweenBase>>> tweens;

  template <math::Lerpable T> class TweenWrapper : public TweenBase {
  private:
    const TweenSystem &system;
    const Entity entity;
    Tween<T> tween;

  public:
    TweenWrapper(const TweenSystem &system, Entity entity, Tween<T> tween)
        : system(system)
        , entity(entity)
        , tween(std::move(tween)) {}

    void update(float dt) override {
      tween.update(dt);
      system.registry->replace(entity, tween.getValue());
    }

    bool isComplete() const override {
      return tween.isComplete();
    }
  };

public:
  static constexpr auto ID = "TweenSystemID";

  TweenSystem(SystemID id, int priority, std::shared_ptr<ComponentRegistry> registry) noexcept
      : System(id, priority)
      , registry(registry){};

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

  template <math::Lerpable T> void tween(Entity entity, Tween<T> tween) {
    tweens.emplace_back(entity, std::make_unique<TweenWrapper<T>>(*this, entity, std::move(tween)));
  }
};
