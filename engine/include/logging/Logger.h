#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include "logging/Formatter.h"
#include "logging/LogLevel.h"
#include <string_view>
#include <source_location>
#include <memory>

namespace GGenie {

namespace log {


class Logger {
private:
  std::shared_ptr<Formatter> formatter;
public:
  Logger(std::shared_ptr<Formatter> formatter);
  void log(LogLevel level, std::string_view message, const std::source_location& location = std::source_location::current());
};

}
}

#endif