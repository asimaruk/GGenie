#include "system/render/RenderSystem.h"
#include "component/Camera.h"
#include "component/Material.h"
#include "component/Mesh.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "glad.h"
#include "graphics/shader.h"
#include "logging/Logg.h"
#include "math/algebras.h"
#include "resources/Resources.h"
#include "utils/hash_utils.h"
#include "utils/narrow.h"
#include <cstddef>
#include <cstdio>
#include <functional>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <optional>
#include <print>
#include <tuple>
#include <utility>

using namespace GGenie;

namespace {
auto materialHash(const Material &material) -> size_t {
  return hash_combined(material.vertexSource, material.fragmentSource);
}

auto toGlmVec3(const Vec3 &vec3) -> glm::vec3 {
  return {vec3.x, vec3.y, vec3.z};
}

auto toGlmQuat(const Quat &quat) -> glm::quat {
  return {quat.w, quat.x, quat.y, quat.z};
}

auto toGlType(const Material::VertexType &type) {
  switch (type) {
  case Material::VertexType::BYTE:
    return GL_BYTE;
  case Material::VertexType::FLOAT:
    return GL_FLOAT;
  case Material::VertexType::INT:
    return GL_INT;
  case Material::VertexType::SHORT:
    return GL_SHORT;
  case Material::VertexType::UNSIGNED_BYTE:
    return GL_UNSIGNED_BYTE;
  case Material::VertexType::UNSIGNED_INT:
    return GL_UNSIGNED_INT;
  case Material::VertexType::UNSIGNED_SHORT:
    return GL_UNSIGNED_SHORT;
  }
}
} // namespace

class RenderSystem::Impl {
public:
  Impl(std::shared_ptr<ComponentRegistry> registry, std::shared_ptr<Resources> resources)
      : registry(registry)
      , resources(resources) {}
  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;

  ~Impl() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (auto &[/*unused*/ _, shaderProgram] : compiledShaders) {
      glDeleteProgram(shaderProgram);
    }
  }

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  void start() {
    log::Logg::info("RenderSystem.start()");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glEnable(GL_DEPTH_TEST);
  }

  void update(float /*dt*/) {
    clear();
    auto cameraIter = registry->getAll<Camera>();
    if (cameraIter.size() == 0) {
      log::Logg::error("No camera components for renderring");
      return;
    }

    auto [camEntity, camera] = cameraIter[0];
    auto cameraTransform = registry->get<Transform>(camEntity);
    if (!cameraTransform) {
      log::Logg::error("Camera entity has no transform");
      return;
    }

    for (const auto [entity, mesh] : registry->getAll<Mesh>()) {
      auto [material, transform] = getOtherComponents(entity);
      if (material.has_value() && transform.has_value()) {
        render(camera, cameraTransform->get(), mesh, material->get(), transform->get());
      }
    }
  }

private:
  std::shared_ptr<ComponentRegistry> registry;
  std::shared_ptr<Resources> resources;
  std::unordered_map<size_t, unsigned int> compiledShaders;
  unsigned int VBO{}, VAO{}, EBO{};

  auto getOtherComponents(Entity entity)
      -> std::tuple<std::optional<std::reference_wrapper<Material>>, std::optional<std::reference_wrapper<Transform>>> {
    auto material = registry->get<Material>(entity);
    auto transform = registry->get<Transform>(entity);
    return std::make_tuple(material, transform);
  }

  void render(
      const Camera &camera,
      const Transform &cameraTransform,
      const Mesh &mesh,
      const Material &material,
      const Transform &transform
  ) {
    if (!isCompiledShader(material)) {
      auto vertexSource = resources->getTextFile(material.vertexSource);
      auto fragmentSource = resources->getTextFile(material.fragmentSource);
      auto shaderProgram = compileShader(vertexSource, fragmentSource); // TODO: compile on another thread
      compiledShaders[materialHash(material)] = shaderProgram;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    auto vertices = mesh.vertices;
    auto indices = mesh.indices;

    glBufferData(GL_ARRAY_BUFFER, narrow<GLsizeiptr>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        narrow<GLsizeiptr>(sizeof(unsigned int) * indices.size()),
        indices.data(),
        GL_STATIC_DRAW
    );

    for (const auto &attr : material.attrs) {
      glVertexAttribPointer(
          attr.index,
          attr.size,
          toGlType(attr.type),
          attr.normalized ? GL_TRUE : GL_FALSE,
          attr.stride,
          attr.pointer
      );
      glEnableVertexAttribArray(attr.index);
    }

    auto shaderProgram = compiledShaders[materialHash(material)];
    glUseProgram(shaderProgram);

    // Матрицы преобразований
    auto view = glm::mat4(1.0F);
    auto projection = glm::mat4(1.0F);
    auto model = glm::mat4(1.0F);

    view = glm::translate(
        view,
        glm::vec3(cameraTransform.translation.x, cameraTransform.translation.y, cameraTransform.translation.z)
    );
    view = view * glm::mat4_cast(toGlmQuat(cameraTransform.rotation));
    view = glm::inverse(view);
    projection = glm::perspective(
        glm::radians(camera.fov),
        static_cast<float>(camera.width) / camera.height,
        camera.near,
        camera.far
    );
    model = glm::translate(model, toGlmVec3(transform.translation));            // translation
    model = model * glm::mat4_cast(toGlmQuat(transform.rotation.normalized())); // rotation
    model = glm::scale(model, toGlmVec3(transform.scale));                      // scale

    // Передача матриц в шейдер
    GLint const modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint const viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint const projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Отрисовка куба
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, narrow<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
  }

  unsigned int compileShader(const std::string_view vertexSource, const std::string_view fragmentSource) {
    return graphics::compileShader(vertexSource, fragmentSource);
  }

  auto isCompiledShader(const Material &material) -> bool {
    size_t const hash = materialHash(material);
    return compiledShaders.contains(hash);
  }

  void clear() {
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F); // NOLINT(readability-magic-numbers)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
};

RenderSystem::RenderSystem(
    SystemID id, int priority, std::shared_ptr<ComponentRegistry> registry, std::shared_ptr<Resources> resources
) noexcept
    : System(id, priority)
    , pimpl(new Impl(registry, resources)) {}

RenderSystem::~RenderSystem() = default;

void RenderSystem::start() {
  pimpl->start();
}

void RenderSystem::update(float dt) {
  pimpl->update(dt);
}
