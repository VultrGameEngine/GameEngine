#pragma once
#include <editor/imgui/window.hpp>

namespace Brick3D::Editor
{
class EntityWindow : public Window
{
  public:
    void Render() override;
};
} // namespace Brick3D::Editor
