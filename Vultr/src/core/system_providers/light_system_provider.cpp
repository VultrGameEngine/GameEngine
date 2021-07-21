#include <core/system_providers/light_system_provider.h>
#include <core/systems/light_system.h>

namespace Vultr::LightSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }
} // namespace Vultr::LightSystem
