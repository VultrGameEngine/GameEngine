#pragma once
#include <core/system_providers/gui_system_provider.h>
#include <gui/framework/window.h>

namespace Vultr
{
class GUISystem
{
  public:
    static void RegisterSystem();
    static void Init(GUI::Window *window);
    static void Update();

  private:
    static GUISystemProvider &GetProvider()
    {
        return *GUISystemProvider::Get();
    }
};
} // namespace Vultr
