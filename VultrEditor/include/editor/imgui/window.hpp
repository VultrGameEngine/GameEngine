#pragma once
#include "element.hpp"
#include <string>
#include <vector>

namespace Brick3D::Editor
{
class Window
{
  public:
    virtual void Render() = 0;
};
}; // namespace Brick3D::Editor
