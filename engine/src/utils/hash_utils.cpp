#include "utils/hash_utils.h"

size_t hash_combined(std::string_view a, std::string_view b) {
    size_t h1 = std::hash<std::string_view>{}(a);
    size_t h2 = std::hash<std::string_view>{}(b);
    
    // Комбинирование хешей (аналогично boost::hash_combine)
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}