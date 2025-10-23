#include "resources/Resources.h"
#include "resources/Loader.h"
#include <memory>

using namespace GGenie;

class Resources::Impl {
public:
  Impl(std::shared_ptr<Loader> loader) : loader(loader) {}
  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;
  ~Impl() = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  std::string getTextFile(std::string_view path) {
    return loader->loadTextFile(path);
  }

private:
  std::shared_ptr<Loader> loader;
};

Resources::Resources(std::shared_ptr<Loader> loader) : pimpl(new Impl(loader)) {}

Resources::~Resources() = default;

std::string Resources::getTextFile(std::string_view path) {
    return pimpl->getTextFile(path);
}