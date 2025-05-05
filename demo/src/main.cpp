#include "component/Mesh.h"
#define GL_SILENCE_DEPRECATION

#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/DefaultWorld.h"
#include "glad.h"
#include "shaders/defaultshaders.h"
#include "window/EngineWindow.h"
#include "window/GlfwEngineWindow.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

int main() {
  EngineWindow *window = new GlfwEngineWindow();
  auto windowWidth = 800;
  auto windowHeight = 600;
  window->initialize(windowWidth, windowHeight, "3D Cube");

  // Инициализация GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "GLAD error!" << std::endl;
    return -1;
  }

  // ECS world setup
  auto registry = std::make_shared<ComponentRegistry>();
  auto world = std::make_shared<DefaultWorld>(registry);
  auto cube = world->createEntity();
  registry->add(cube, Mesh::cube());
  registry->add(cube, Shader(ShaderSource::DEFAULT_VERTEX, ShaderSource::DEFAULT_FRAGMENT));
  registry->add(cube, Transform());

  auto shader = registry->get<Shader>(cube);

  // Компиляция шейдеров
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderSource = shader->getVertex().data();
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderSource = shader->getFragment().data();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Удаляем шейдеры (они уже в программе)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Создание буферов
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  auto cubeMesh = registry->get<Mesh>(cube);
  auto vertices = registry->get<Mesh>(cube)->getVertices();
  auto indices = cubeMesh->getIndices();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Разрешаем тест глубины (для 3D)
  glEnable(GL_DEPTH_TEST);

  // Цикл рендеринга
  while (!window->shouldClose()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Матрицы преобразований
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Вращение куба
    float time = glfwGetTime();
    auto transform = registry->get<Transform>(cube);
    transform->setRotatation(time * glm::radians(50.0f), {0.5f, 1.0f, 0.0f});
    transform->setScale(Vec3::ONE + Vec3::fill((1 + std::sin(time)) / 2.f));
    auto model = transform->getMatrix().getData();
    view = glm::translate(view, glm::vec3(0.0f, -1.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowWidth) / windowHeight, 0.1f, 100.0f);

    // Передача матриц в шейдер
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Отрисовка куба
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Обмен буферов и обработка событий
    window->swapBuffers();
    window->pollEvents();
  }

  // Очистка
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  delete window;
  return 0;
}