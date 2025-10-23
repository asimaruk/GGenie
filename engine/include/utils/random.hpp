#ifndef UTILS_RANDOM_H
#define UTILS_RANDOM_H

#include <random>

namespace GGenie {

template <typename T> auto random_0_to_1() -> T {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return std::generate_canonical<T, 10>(gen);
}

template <typename T> auto randomDist(std::uniform_real_distribution<T> distr) -> T {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return distr(gen);
}

} // namespace GGenie

#endif
