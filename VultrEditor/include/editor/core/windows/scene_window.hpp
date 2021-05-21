#pragma once
#include <editor/imgui/window.hpp>

class SceneWindow : public Window
{
  public:
    SceneWindow();
    void Render() override;
};
