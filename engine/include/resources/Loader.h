#ifndef RESOURCES_LOADER_H
#define RESOURCES_LOADER_H

#include "config/Config.h"
#include <memory>
#include <string>

class Loader {
public:
  Loader(std::shared_ptr<Config> config);
  Loader(const Loader &) = delete;
  Loader(Loader &&) = delete;
  ~Loader();

  auto operator=(const Loader &) -> Loader & = delete;
  auto operator=(Loader &&) -> Loader & = delete;

  std::string loadTextFile(std::string_view path);

private:
  class Impl;
  std::unique_ptr<Impl> pimpl;
};

#endif
