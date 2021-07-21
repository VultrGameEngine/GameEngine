#include <core/system_providers/controller_system_provider.h>

namespace Vultr::ControllerSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }
} // namespace Vultr::ControllerSystem
