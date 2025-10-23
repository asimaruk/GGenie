#include "resources/Loader.h"
#include "config/Config.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

using namespace GGenie;

class Loader::Impl {
public:
  Impl(std::shared_ptr<Config> config) : config(config) {}
  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;
  ~Impl() = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  std::string loadTextFile(std::string_view path) {
    const auto filePath = std::filesystem::path(config->resPath) / path;
    if (!std::filesystem::exists(filePath)) {
      throw std::runtime_error(std::format("File does not exist: {}", filePath.string()));
    }
    if (!std::filesystem::is_regular_file(filePath)) {
      throw std::runtime_error(std::format("Not a regular file: {}", filePath.string()));
    }

    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

private:
  std::shared_ptr<Config> config;
};

Loader::Loader(std::shared_ptr<Config> config) : pimpl(new Impl(config)) {}

Loader::~Loader() = default;

std::string Loader::loadTextFile(std::string_view path) {
  return pimpl->loadTextFile(path);
}
