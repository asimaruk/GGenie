#ifndef UTILS_HASH_UTILS_H
#define UTILS_HASH_UTILS_H

#include <string_view>

namespace GGenie {

auto hash_combined(std::string_view a, std::string_view b) -> size_t;

}

#endif
