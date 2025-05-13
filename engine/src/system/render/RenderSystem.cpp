#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "glad.h"
#include "math/Quat.h"
#include "system/render/RenderSystem.h"
#include "utils/hash_utils.h"
#include <__ostream/print.h>
#include <cstddef>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <tuple>

size_t shaderHash(const Shader &shader) {
  return hash_combined(shader.getVertex(), shader.getFragment());
}

glm::vec3 toGlmVec3(const Vec3 &v3) { return glm::vec3(v3.x, v3.y, v3.z); }

glm::quat toGlmQuat(const Quat &q) { return glm::quat(q.w, q.x, q.y, q.z); }

RenderSystem::RenderSystem(SystemID id, int priority,
                           std::shared_ptr<ComponentRegistry> registry) noexcept
    : System(id, priority), registry(registry) {};

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

void RenderSystem::update(float dt) {
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
      render(camera, mesh, shader->get(), transform->get());
    }
  }
}

std::tuple<std::optional<std::reference_wrapper<Shader>>,
           std::optional<std::reference_wrapper<Transform>>>
RenderSystem::getOtherComponents(Entity entity) {
  auto shader = registry->get<Shader>(entity);
  auto transform = registry->get<Transform>(entity);
  return std::make_tuple(shader, transform);
}

void RenderSystem::clear() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::render(const Camera &camera, const Mesh &mesh,
                          const Shader &shader, const Transform &transform) {
  if (!isCompiledShader(shader)) {
    compileShader(shader); // TODO: compile on another thread
  }

  auto vertices = mesh.getVertices();
  auto indices = mesh.getIndices();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  auto shaderProgram = compiledShaders[shaderHash(shader)];
  glUseProgram(shaderProgram);

  // Матрицы преобразований
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  glm::mat4 model = glm::mat4(1.0f);

  view = glm::translate(view, glm::vec3(0.0f, -1.0f, -5.0f));
  projection =
      glm::perspective(glm::radians(camera.fov),
                       static_cast<float>(camera.width) / camera.height,
                       camera.near, camera.far);
  model =
      glm::translate(model, toGlmVec3(transform.translation));   // translation
  model = model * glm::mat4_cast(toGlmQuat(transform.rotation.normalized())); // rotation
  model = glm::scale(model, toGlmVec3(transform.scale));         // scale

  // Передача матриц в шейдер
  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // Отрисовка куба
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderSystem::compileShader(const Shader &shader) {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderSource = shader.getVertex().data();
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderSource = shader.getFragment().data();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Удаляем шейдеры (они уже в программе)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  compiledShaders[shaderHash(shader)] = shaderProgram;
}

bool RenderSystem::isCompiledShader(const Shader &shader) {
  size_t hash = shaderHash(shader);
  return compiledShaders.contains(hash);
}
