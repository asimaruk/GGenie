#include "system/control/FirstPersonViewControlSystem.h"
#include "component/Move.h"
#include "component/Transform.h"
#include "ecs/System.h"
#include "math/algebras.h"
#include "system/control/FPVEvent.h"
#include "system/event/Event.h"
#include <numbers>
#include <print>

class FirstPersonViewControlSystem::Impl {
private:
  std::shared_ptr<ComponentRegistry> registry;
  Entity controlledEntity = -1;
  Vec3 direction = Vec3::ZERO;
  float xViewDiff = 0;
  float yViewDiff = 0;
  float sensivity = 0.001;

public:
  Impl(std::shared_ptr<ComponentRegistry> registry) : registry(registry) {}

  void setControlledEntity(Entity entity) noexcept {
    controlledEntity = entity;
  }

  void update(float dt) {
    const auto transform = registry->get<Transform>(controlledEntity);
    const auto move = registry->get<Move>(controlledEntity);

    if (!transform || !move || (direction == Vec3::ZERO && xViewDiff == 0 && yViewDiff == 0)) {
      return;
    }

    auto yaw = Quat::fromAxisAngle(Vec3{0, 1, 0}, -xViewDiff * sensivity);
    auto pitch = Quat::fromAxisAngle(Vec3{1, 0, 0}, -yViewDiff * sensivity);
    auto rotation = yaw * transform->get().rotation * pitch;
    auto moveVector = direction.normalize().rotate(rotation);
    auto translation = transform->get().translation + moveVector * move->get().speed * dt;
    registry->replace<Transform>(
        controlledEntity,
        Transform{
            .translation = translation,
            .rotation = rotation,
            .scale = transform->get().scale,
        }
    );
    xViewDiff = 0;
    yViewDiff = 0;
  }

  void onFPVMoveEvent(const FPVMoveEvent &event) {
    direction = event.direction;
  }

  void onFPVViewEvent(const FPVViewEvent &event) {
    xViewDiff = event.xViewDiff;
    yViewDiff = event.yViewDiff;
  }
};

FirstPersonViewControlSystem::FirstPersonViewControlSystem(
    SystemID systemId,
    int priority,
    std::shared_ptr<ComponentRegistry> registry,
    std::shared_ptr<EventSystem> eventSystem
)
    : System(systemId, priority)
    , pimpl(new Impl(registry)) {
  eventSystem->on<FPVMoveEvent>(
      [this](const FPVMoveEvent &event)
      {
        pimpl->onFPVMoveEvent(event);
      }
  );
  eventSystem->on<FPVViewEvent>(
      [this](const FPVViewEvent &event)
      {
        pimpl->onFPVViewEvent(event);
      }
  );
}

void FirstPersonViewControlSystem::setControlledEntity(Entity entity) noexcept {
  pimpl->setControlledEntity(entity);
}

void FirstPersonViewControlSystem::start() {}

void FirstPersonViewControlSystem::update(float dt) {
  pimpl->update(dt);
}

FirstPersonViewControlSystem::~FirstPersonViewControlSystem() {}
