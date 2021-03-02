#pragma once
#include "imgui/window.hpp"
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

  private:
    std::vector<Window *> windows;
    unsigned int dockspace = 1;
};
} // namespace Vultr::Editor
