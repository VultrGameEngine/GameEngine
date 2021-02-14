#pragma once
#include "imgui/window.hpp"
#include <ecs/entity/entity.hpp>
#include <vector>

namespace Brick3D::Editor
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

  private:
    std::vector<Window *> windows;
    unsigned int dockspace = 1;
    Entity selected = Entity(-1);
};
} // namespace Brick3D::Editor
