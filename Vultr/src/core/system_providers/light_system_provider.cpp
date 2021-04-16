#include <core/system_providers/light_system_provider.h>
#include <core/systems/light_system.h>

namespace Vultr
{
void LightSystemProvider::OnCreateEntity(Entity entity)
{
    LightSystem::OnCreateEntity(entity);
}

void LightSystemProvider::OnDestroyEntity(Entity entity)
{
}
} // namespace Vultr
