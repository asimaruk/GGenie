#include "logging/Formatter.h"
#include "logging/LogLevel.h"
#include <string>
#include <format>
#include <source_location>
#include <filesystem>

using namespace GGenie;
using namespace GGenie::log;

auto getLevelString(LogLevel level) -> std::string {
  static const std::array<std::string_view, 6> levels = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"
  };
  return std::string(levels[static_cast<size_t>(level)]);
}

Formatter::Formatter(std::string_view rootDir) : rootDir(rootDir) {}

auto Formatter::format(LogLevel level, std::string_view message, const std::source_location& location) -> std::string {
  auto relativePath = std::filesystem::relative(location.file_name(), rootDir);
  return std::format(
    "[{}][{}:{}:{}] {}", 
    getLevelString(level), 
    relativePath.c_str(),
    location.line(), 
    location.column(),
    message
  );
}