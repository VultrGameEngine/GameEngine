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
    virtual void RegisterComponents(Vultr::Engine *e) = 0;
    virtual void Init(Vultr::Engine *e) = 0;
    virtual void Update(Vultr::Engine *e, const Vultr::UpdateTick &tick) = 0;
    virtual void Flush(Vultr::Engine *e) = 0;
    virtual void SetImGuiContext(ImGuiContext *context) = 0;
    virtual ~Game()
    {
    }
};
