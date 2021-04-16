#pragma once
#include "element.hpp"
#include <string>
#include <vector>

namespace Vultr::Editor
{
class Window
{
  public:
    virtual void Render() = 0;
};
}; // namespace Vultr::Editor
