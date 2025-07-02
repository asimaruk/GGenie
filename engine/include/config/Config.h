#pragma once

#include <string>
#include <iostream>
#include <format>

struct Config {
    std::string resPath;

    std::ostream &operator<<(std::ostream &ostream) const;
};

template<>
struct std::formatter<Config> {
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') {
            throw std::format_error("invalid format specifier for Config");
        }
        return it;
    }

    auto format(const Config& c, std::format_context& ctx) const {
        return std::format_to(ctx.out(),
            "{{resPath: {}}}",
            c.resPath);
    }
};