#include "system/render/RenderSystem.h"
#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "glad.h"
#include "math/algebras.h"
#include "utils/hash_utils.h"
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
#include <glm/trigonometric.hpp>
#include <memory>
#include <optional>
#include <print>
#include <tuple>
#include <utility>

namespace {
auto shaderHash(const Shader &shader) -> size_t {
  return hash_combined(shader.vertexSource, shader.fragmentSource);
}

auto toGlmVec3(const Vec3 &vec3) -> glm::vec3 {
  return {vec3.x, vec3.y, vec3.z};
}

auto toGlmQuat(const Quat &quat) -> glm::quat {
  return {quat.w, quat.x, quat.y, quat.z};
}
} // namespace

RenderSystem::RenderSystem(SystemID id, int priority, std::shared_ptr<ComponentRegistry> registry) noexcept
    : System(id, priority)
    , registry(std::move(registry))
    , VBO{}
    , VAO{}
    , EBO{} {}

RenderSystem::~RenderSystem() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  for (auto &[_, shaderProgram] : compiledShaders) {
    glDeleteProgram(shaderProgram);
  }
}

void RenderSystem::start() {
  std::println("RenderSystem.start()");
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glEnable(GL_DEPTH_TEST);
}

void RenderSystem::update(float /*dt*/) {
  clear();
  auto cameraIter = registry->getAll<Camera>();
  if (cameraIter.size() == 0) {
    std::println(stderr, "No camera components for renderring");
    return;
  }

  auto [camEntity, camera] = cameraIter[0];
  auto cameraTransform = registry->get<Transform>(camEntity);
  if (!cameraTransform) {
    std::println(stderr, "Camera entity has no transform");
    return;
  }

  for (const auto [entity, mesh] : registry->getAll<Mesh>()) {
    auto [shader, transform] = getOtherComponents(entity);
    if (shader.has_value() && transform.has_value()) {
      render(camera, cameraTransform->get(), mesh, shader->get(), transform->get());
    }
  }
}

auto RenderSystem::getOtherComponents(Entity entity)
    -> std::tuple<std::optional<std::reference_wrapper<Shader>>, std::optional<std::reference_wrapper<Transform>>> {
  auto shader = registry->get<Shader>(entity);
  auto transform = registry->get<Transform>(entity);
  return std::make_tuple(shader, transform);
}

void RenderSystem::clear() {
  glClearColor(0.2F, 0.3F, 0.3F, 1.0F); // NOLINT(readability-magic-numbers)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::render(
    const Camera &camera,
    const Transform &cameraTransform,
    const Mesh &mesh,
    const Shader &shader,
    const Transform &transform
) {
  if (!isCompiledShader(shader)) {
    compileShader(shader); // TODO: compile on another thread
  }

  auto vertices = mesh.vertices;
  auto indices = mesh.indices;

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  auto shaderProgram = compiledShaders[shaderHash(shader)];
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
  unsigned int const modelLoc = glGetUniformLocation(shaderProgram, "model");
  unsigned int const viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int const projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // Отрисовка куба
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void RenderSystem::compileShader(const Shader &shader) {
  unsigned int const vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderSource = shader.vertexSource.data();
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  unsigned int const fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderSource = shader.fragmentSource.data();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  unsigned int const shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Удаляем шейдеры (они уже в программе)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  compiledShaders[shaderHash(shader)] = shaderProgram;
}

auto RenderSystem::isCompiledShader(const Shader &shader) -> bool {
  size_t const hash = shaderHash(shader);
  return compiledShaders.contains(hash);
}
