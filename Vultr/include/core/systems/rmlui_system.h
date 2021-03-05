#pragma once
#include <core/system_providers/rmlui_system_provider.h>

namespace Vultr
{
class RmlUiSystem
{
  public:
    static void RegisterSystem();
    static void Update();

  private:
    static RmlUiSystemProvider &GetProvider()
    {
        return *RmlUiSystemProvider::Get();
    }
};
} // namespace Vultr
