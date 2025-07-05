#include "Interpolator.hpp"
#include "math/lerp.hpp"
#include "Tween.hpp"

template <math::Lerpable T> class TweenBuilder {
public:
  TweenBuilder(T start, T end) : start(start), end(end) {}

  Tween<T> create() {
    return Tween(_duration, start, end, _interpolator);
  }

  TweenBuilder<T> &duration(float duration) {
    _duration = duration;
    return *this;
  }

  TweenBuilder<T> &interpolator(Interpolator interpolator) {
    _interpolator = interpolator;
    return *this;
  }

private:
  const T start, end;
  float _duration = 0;
  Interpolator _interpolator = Interpolator::LinearInterpolator;
};
