#ifndef SYSTEM_STATS_STATSSYSTEM_H
#define SYSTEM_STATS_STATSSYSTEM_H

#include "config/Config.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/System.h"
#include "window/EngineWindow.h"
#include <memory>

class StatsSystem : public System {
public:
  static constexpr auto ID = "StatsSystemID";
  StatsSystem(SystemID id, int priority, const Config &config, const EngineWindow &window);
  ~StatsSystem();
  void start() override;
  void update(float dt) override;

private:
  class Impl;
  std::unique_ptr<Impl> pimpl;
};

#endif
