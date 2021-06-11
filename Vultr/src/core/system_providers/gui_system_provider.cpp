#include <core/system_providers/gui_system_provider.h>

namespace Vultr::GUISystem
{
    Component &get_provider()
    {
        return *get_global_system_provider<Component>();
    }
} // namespace Vultr::GUISystem
