#pragma once
#include <core/system_providers/gui_system_provider.h>
#include <gui/framework/window.h>
#include <core/system_providers/input_system_provider.h>
#include <core/models/update_tick.h>

namespace Vultr
{
class GUISystem
{
  public:
    static void RegisterSystem();
    static void Init(GUI::Window *window);
    static void Update(UpdateTick tick);
    static bool ReceiveMouseEvent(Input::MouseInputEvent event);
    static bool ReceiveMouseButtonEvent(Input::MouseButtonInputEvent event);

  private:
    static GUISystemProvider &GetProvider()
    {
        return *GUISystemProvider::Get();
    }
};
} // namespace Vultr
