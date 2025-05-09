#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/DefaultWorld.h"
#include "ecs/System.h"
#include "glad.h"
#include "system/render/defaultshaders.h"
#include "system/render/RenderSystem.h"
#include "window/EngineWindow.h"
#include "window/GlfwEngineWindow.h"
#include <__ostream/print.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#define GL_SILENCE_DEPRECATION

int main() {
  std::println("Very start");
  EngineWindow *window = new GlfwEngineWindow();
  auto windowWidth = 800;
  auto windowHeight = 600;
  window->initialize(windowWidth, windowHeight, "GGenie");

  // Инициализация GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println(std::cerr, "GLAD error!");
    return -1;
  }

  // ECS world setup
  auto registry = std::make_shared<ComponentRegistry>();
  auto world = std::make_shared<DefaultWorld>(registry);
  auto renderSystem = std::make_shared<RenderSystem>(
      RenderSystem::ID, System::PRIORITY_LOW, registry);
  world->registerSystem(renderSystem);
  auto cube = world->createEntity();
  registry->add(cube, Mesh::cube());
  registry->add(cube, Shader(ShaderSource::DEFAULT_VERTEX,
                             ShaderSource::DEFAULT_FRAGMENT));
  registry->add(cube, Transform());
  auto camera = world->createEntity();
  auto cameraTransform = Transform();
  cameraTransform.translation = Vec3(0, -1, -5);
  registry->add(camera, cameraTransform);
  registry->add(camera, Camera(windowWidth, windowHeight, 45, 0.1, 100));

  std::println("World setup");

  float lastTime = glfwGetTime();
  while (!window->shouldClose()) {
    float time = glfwGetTime();
    float dt = time - lastTime;
    lastTime = time;

    world->update(dt);
    window->swapBuffers();
    window->pollEvents();
  }

  delete window;
  return 0;
}