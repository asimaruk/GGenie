#ifndef SYSTEM_RENDER_RENDERSYSTEM_H
#define SYSTEM_RENDER_RENDERSYSTEM_H

#include "component/Camera.h"
#include "component/Material.h"
#include "component/Mesh.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "resources/Resources.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include <unordered_map>

class RenderSystem : public System {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  static constexpr auto ID = "RenderSystemID";

  RenderSystem(
      SystemID id, int priority, std::shared_ptr<ComponentRegistry> registry, std::shared_ptr<Resources> resources
  ) noexcept;
  RenderSystem(const RenderSystem &) = delete;
  RenderSystem(RenderSystem &&) = delete;
  ~RenderSystem() override;

  auto operator=(const RenderSystem &) -> RenderSystem & = delete;
  auto operator=(RenderSystem &&) -> RenderSystem & = delete;

  void start() override;
  void update(float dt) override;
};

#endif
