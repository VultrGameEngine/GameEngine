#pragma once
namespace Vultr
{
    struct Engine;
}
#include <core/models/update_tick.h>
#include <imgui/imgui.h>

class Game
{
  public:
    virtual void RegisterComponents() = 0;
    virtual void Init() = 0;
    virtual void Update(const Vultr::UpdateTick &tick) = 0;
    virtual void Flush() = 0;
    virtual void SetImGuiContext(ImGuiContext *context) = 0;
    virtual ~Game()
    {
    }
};
