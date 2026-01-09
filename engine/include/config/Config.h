#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <format>
#include <iostream>
#include <string>

namespace GGenie {

struct Config {
  std::string resPath;

  auto operator<<(std::ostream &ostream) const -> std::ostream &;
};

} // namespace GGenie

template <> struct std::formatter<GGenie::Config> {
  constexpr auto parse(std::format_parse_context &ctx) {
    const auto iter = ctx.begin();
    const auto end = ctx.end();
    if (iter != end && *iter != '}') {
      throw std::format_error("invalid format specifier for Config");
    }
    return iter;
  }

  auto format(const GGenie::Config &config, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{{resPath: {}}}", config.resPath);
  }
};

#endif
