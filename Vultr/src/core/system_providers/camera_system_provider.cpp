#include <core/system_providers/camera_system_provider.h>
#include <core/systems/camera_system.h>

namespace Vultr::CameraSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }

} // namespace Vultr::CameraSystem
