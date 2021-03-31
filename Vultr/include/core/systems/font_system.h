#pragma once
#include <core/system_providers/font_system_provider.h>

namespace Vultr
{
class FontSystem
{
  public:
    static void RegisterSystem();
    static void Init();
    static void PreloadFont(const std::string &path);

  private:
    static FontSystemProvider &GetProvider()
    {
        std::shared_ptr<FontSystemProvider> provider = FontSystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr
