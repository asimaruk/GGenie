#include "logging/Logg.h"
#include "logging/Logger.h"
#include "logging/LogLevel.h"
#include <string_view>
#include <source_location>

using namespace GGenie;
using namespace GGenie::log;

std::shared_ptr<Logger> GGenie::log::Logg::defaultLogger;

void GGenie::log::Logg::setDefault(std::shared_ptr<Logger> logger) {
  defaultLogger = logger;
}

void GGenie::log::Logg::trace(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Trace, message, location);
}

void GGenie::log::Logg::debug(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Debug, message, location);
}

void GGenie::log::Logg::info(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Info, message, location);
}

void GGenie::log::Logg::warning(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Warning, message, location);
}

void GGenie::log::Logg::error(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Error, message, location);
}

void GGenie::log::Logg::critical(std::string_view message, const std::source_location& location) {
  defaultLogger->log(LogLevel::Critical, message, location);
}
