#ifndef RESOURCES_RESOURCES_H
#define RESOURCES_RESOURCES_H

#include "Loader.h"
#include "config/Config.h"
#include <memory>
#include <string>

namespace GGenie {

class Resources {
public:
  Resources(std::shared_ptr<Loader> loader);
  Resources(const Resources &) = delete;
  Resources(Resources &&) = delete;
  ~Resources();

  auto operator=(const Resources &) -> Resources & = delete;
  auto operator=(Resources &&) -> Resources & = delete;

  std::string getTextFile(std::string_view path);

private:
  class Impl;
  std::unique_ptr<Impl> pimpl;
};

} // namespace GGenie

#endif
