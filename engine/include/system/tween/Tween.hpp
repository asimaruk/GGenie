#pragma once

#include "Interpolator.hpp"
#include <algorithm>

class TweenBase {
public:
  virtual ~TweenBase() = default;
  virtual void update(float dt) = 0;
  virtual bool isComplete() const = 0;
};

template <Lerpable T> class Tween : public TweenBase {
private:
  const float duration;
  const T start, end;
  const Interpolator interpolator;
  float time = 0;

public:
  Tween<T>(float duration, T start, T end,
           const Interpolator &interpolator =
               Interpolator::LinearInterpolator) noexcept
      : duration(duration), start(start), end(end), interpolator(interpolator) {
  }

  void update(float dt) override {
    time += dt;
  }

  bool isComplete() const override { return time >= duration; }

  T getValue() {
    auto t = std::min(1.f, time / duration);
    return interpolator(start, end, t);
  }
};
