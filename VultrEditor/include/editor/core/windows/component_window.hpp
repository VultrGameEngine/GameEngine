#pragma once
#include <editor/imgui/window.hpp>
#include <imgui/imgui.h>

namespace Vultr::Editor
{
class ComponentWindow : public Window
{
  public:
    void Render() override;
    template <typename T> void RenderComponent(T &component)
    {
        ImGui::Text("Component Window Not Defined!");
    }
};
} // namespace Vultr::Editor
