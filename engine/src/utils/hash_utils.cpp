#include "utils/hash_utils.h"
#include <cstddef>
#include <functional>
#include <string_view>

auto hash_combined(std::string_view a, std::string_view b) -> size_t {
  size_t const hash1 = std::hash<std::string_view>{}(a);
  size_t const hash2 = std::hash<std::string_view>{}(b);

  // Комбинирование хешей (аналогично boost::hash_combine)
  return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6U) + (hash1 >> 2U)); // NOLINT(readability-magic-numbers)
}
