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
    static bool ReceiveScrollEvent(Input::ScrollInputEvent event);

  private:
    static GUISystemProvider &GetProvider()
    {
        std::shared_ptr<GUISystemProvider> provider = GUISystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr
