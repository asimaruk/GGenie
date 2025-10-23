#include "logging/Formatter.h"
#include "logging/Logger.h"
#include "logging/LogLevel.h"
#include <string>
#include <format>
#include <print>
#include <iostream>

using namespace GGenie;
using namespace GGenie::log;

Logger::Logger(std::shared_ptr<Formatter> formatter) : formatter(formatter) {};
void Logger::log(LogLevel level, std::string_view message, const std::source_location& location) {
#ifndef NDEBUG
  std::cout << formatter->format(level, message, location) << std::endl;
#endif
}