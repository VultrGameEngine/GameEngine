#include <core/system_providers/font_system_provider.h>

namespace Vultr::FontSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }
} // namespace Vultr::FontSystem
