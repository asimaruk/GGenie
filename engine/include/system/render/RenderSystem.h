#ifndef SYSTEM_RENDER_RENDERSYSTEM_H
#define SYSTEM_RENDER_RENDERSYSTEM_H

#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include <unordered_map>

class RenderSystem : public System {
private:
  std::shared_ptr<ComponentRegistry> registry;
  std::unordered_map<size_t, unsigned int> compiledShaders;
  unsigned int VBO, VAO, EBO;

  auto getOtherComponents(Entity entity)
      -> std::tuple<std::optional<std::reference_wrapper<Shader>>, std::optional<std::reference_wrapper<Transform>>>;
  void render(
      const Camera &camera,
      const Transform &cameraTransform,
      const Mesh &mesh,
      const Shader &shader,
      const Transform &transform
  );
  void compileShader(const Shader &shader);
  auto isCompiledShader(const Shader &shader) -> bool;
  void clear();

public:
  static constexpr auto ID = "RenderSystemID";

  RenderSystem(SystemID id, int priority, std::shared_ptr<ComponentRegistry> registry) noexcept;
  ~RenderSystem();

  void start() override;
  void update(float dt) override;
};

#endif
