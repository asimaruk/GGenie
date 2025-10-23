#include "config/Config.h"
#include <iostream>

using namespace GGenie;

auto Config::operator<<(std::ostream &ostream) const -> std::ostream & {
  ostream << "{resPath: " << resPath << "}";
  return ostream;
}
