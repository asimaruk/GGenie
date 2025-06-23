#include "config/Config.h"
#include <string>
#include <iostream>

std::ostream& Config::operator<<(std::ostream& os) const {
    os << "{resPath: " << resPath << "}";
    return os;
}