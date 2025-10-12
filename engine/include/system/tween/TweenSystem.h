#pragma once

#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <memory>
#include <utility>
#include <vector>

class TweenSystem : public System {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;
  
public:
  static constexpr auto ID = "TweenSystemID";

  TweenSystem(const SystemID id, const int priority, std::shared_ptr<ComponentRegistry> registry) noexcept;
  TweenSystem(const TweenSystem &) = delete;
  TweenSystem(TweenSystem &&) = delete;
  ~TweenSystem() override;

  auto operator=(const TweenSystem &) -> TweenSystem & = delete;
  auto operator=(TweenSystem &&) -> TweenSystem & = delete;

  void start() override;
  void update(float dt) override;
};
