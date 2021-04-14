#pragma once
#include <string>

namespace Vultr
{
class Generator
{
  public:
    Generator(const std::string &path, unsigned int last_build_date);
    ~Generator();

  private:
};
} // namespace Vultr
