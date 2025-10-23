#ifndef LOGGING_FORMATTER_H
#define LOGGING_FORMATTER_H

#include "logging/LogLevel.h"
#include <string>
#include <source_location>

namespace GGenie {
    
namespace log {

class Formatter {
private:
  std::string_view rootDir;
public:
  Formatter(std::string_view rootDir);
  auto format(LogLevel level, std::string_view message, const std::source_location& location) -> std::string;
};

}
}

#endif