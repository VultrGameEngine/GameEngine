#include <system_providers/controller_system_provider.h>
#include <systems/controller_system.h>

void ControllerSystemProvider::OnCreateEntity(Entity entity)
{
    ControllerSystem::OnCreateEntity(entity);
}

void ControllerSystemProvider::OnDestroyEntity(Entity entity)
{
    ControllerSystem::OnDestroyEntity(entity);
}
