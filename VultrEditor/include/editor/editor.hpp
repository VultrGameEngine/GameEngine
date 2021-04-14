#pragma once
#include "imgui/window.hpp"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/entity/entity.hpp>
#include <vector>

namespace Vultr::Editor
{
class Editor
{
  public:
    Editor();
    ~Editor();
    static Editor *Get()
    {
        static Editor *instance;
        if (instance == nullptr)
        {
            instance = new Editor();
        }
        return instance;
    }
    static unsigned int GetDockSpace();
    static void Render();
    Entity selected_entity;
    ImGuizmo::OPERATION current_operation = ImGuizmo::OPERATION::TRANSLATE;

  private:
    std::vector<Window *> windows;
    unsigned int dockspace = 1;
};
} // namespace Vultr::Editor
