#include "adapter/event/GLFWEventsAdapter.h"
#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Move.h"
#include "component/Shader.h"
#include "component/Transform.h"
#include "component/TransformTween.h"
#include "config/Config.h"
#include "ecs/ComponentRegistry.hpp"
#include "ecs/DefaultWorld.h"
#include "ecs/System.h"
#include "glad.h"
#include "math/algebras.h"
#include "system/control/FirstPersonViewControlSystem.h"
#include "system/event/EventSystem.hpp"
#include "system/input/GLFWInputSystem.h"
#include "system/render/RenderSystem.h"
#include "system/render/defaultshaders.h"
#include "system/stats/StatsSystem.h"
#include "system/tween/TweenSystem.h"
#include "window/GlfwEngineWindow.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <numbers>
#include <print>
#include <random>

int main(int argc, char **argv) {

#ifdef NDEBUG
  std::println("Release configuration!");
#else
  std::println("Debug configuration!");
#endif

  GlfwEngineWindow window = GlfwEngineWindow();
  auto windowWidth = 800;
  auto windowHeight = 600;
  window.initialize(windowWidth, windowHeight, "GGenie");

  // Инициализация GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println(std::cerr, "GLAD error!");
    return -1;
  }

  auto curPath = std::filesystem::current_path();
  auto exePath = (curPath / argv[0] / "../resources").lexically_normal();
  auto config = Config{
      .resPath = exePath.string(),
  };
  std::println("Config {}", config);

  // ECS world setup
  auto registry = std::make_shared<ComponentRegistry>();
  auto world = std::make_shared<DefaultWorld>(registry);

  // Systems setup
  auto eventSystem = std::make_shared<EventSystem>();
  auto glfwEventsEdapter = GLFWEventsAdapter(false);
  glfwEventsEdapter.attach(eventSystem);
  auto renderSystem = std::make_shared<RenderSystem>(RenderSystem::ID, System::PRIORITY_LOW, registry);
  auto tweenSystem = std::make_shared<TweenSystem>(TweenSystem::ID, System::PRIORITY_MEDIUM, registry);
  auto inputSystem = std::make_shared<GLFWInputSystem>(GLFWInputSystem::ID, System::PRIORITY_HIGH, eventSystem);
  auto controlSystem = std::make_shared<FirstPersonViewControlSystem>(
      FirstPersonViewControlSystem::ID,
      System::PRIORITY_HIGH + 1,
      registry,
      eventSystem
  );
  auto statsSystem = std::make_shared<StatsSystem>(StatsSystem::ID, System::PRIORITY_LOW + 1, config, window);
  world->registerSystem(renderSystem);
  world->registerSystem(tweenSystem);
  world->registerSystem(inputSystem);
  world->registerSystem(controlSystem);
  world->registerSystem(statsSystem);

  // Entities and component setup
  auto cube = world->createEntity();
  registry->add(cube, Mesh::cube());
  registry->add(cube, Shader(shaders::DEFAULT_VERTEX, shaders::DEFAULT_FRAGMENT, shaders::DEFAULT_ATTRS));
  registry->add(cube, Transform());

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-25.f, 25.f);
  for (int i = 0; i < 100; ++i) {
    auto cubeCopy = world->copyEntity(cube);
    registry->replace(cubeCopy, Transform{.translation{dist(gen), dist(gen), dist(gen)}});
  }

  auto camera = world->createEntity();
  auto cameraTransform = Transform{.translation{0, 1, 5}};
  registry->add(camera, cameraTransform);
  registry->add(camera, Camera(windowWidth, windowHeight, 45, 0.1, 100));
  registry->add(camera, Move{.speed = 10});
  controlSystem->setControlledEntity(camera);

  std::println("World setup");

  auto cubeTransform = registry->get<Transform>(cube);
  if (cubeTransform.has_value()) {
    registry->add(
        cube,
        TransformTween{
            .duration = 5.f,
            .from =
                Transform{
                    .translation = cubeTransform->get().translation,
                    .rotation = Quat::fromAxisAngle({0, 1, 0}, std::numbers::pi * 0.001),
                    .scale = cubeTransform->get().scale,
                },
            .to =
                Transform{
                    .translation = cubeTransform->get().translation,
                    .rotation = Quat::fromAxisAngle({0, 1, 0}, std::numbers::pi * 2),
                    .scale = cubeTransform->get().scale,
                },
            .time = 0.f,
            .repeat = TransformTween::INFINITY_REPEAT,
        }
    );
  }

  float lastTime = glfwGetTime();
  while (!window.shouldClose()) {
    const float time = glfwGetTime();
    const float dt = time - lastTime;
    lastTime = time;

    world->update(dt);
    window.swapBuffers();
  }

  return 0;
}
