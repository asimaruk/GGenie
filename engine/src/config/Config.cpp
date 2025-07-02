#include "config/Config.h"
#include <iostream>

auto Config::operator<<(std::ostream &ostream) const -> std::ostream & {
  ostream << "{resPath: " << resPath << "}";
  return ostream;
}
