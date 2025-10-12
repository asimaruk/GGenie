#pragma once

#include <cmath>
#include <numbers>

// https://easings.net
namespace easing {

const auto LINEAR = [](float t)
{
  return t;
};
const auto EASE_OUT_SIN = [](float t)
{
  return std::sin((t * std::numbers::pi) / 2);
};

} // namespace easing
