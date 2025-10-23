#ifndef LOGGING_LOGG_H
#define LOGGING_LOGG_H

#include "logging/Logger.h"
#include <memory>
#include <string_view>
#include <source_location>

namespace GGenie {
    
namespace log {

class Logg {
public:
  static void setDefault(std::shared_ptr<Logger> logger);
  static void trace(std::string_view message, const std::source_location& location = std::source_location::current());
  static void debug(std::string_view message, const std::source_location& location = std::source_location::current());
  static void info(std::string_view message, const std::source_location& location = std::source_location::current());
  static void warning(std::string_view message, const std::source_location& location = std::source_location::current());
  static void error(std::string_view message, const std::source_location& location = std::source_location::current());
  static void critical(std::string_view message, const std::source_location& location = std::source_location::current());
private:
  static std::shared_ptr<Logger> defaultLogger;
};

}
}

#endif