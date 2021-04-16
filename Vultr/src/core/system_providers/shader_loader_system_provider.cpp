#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr
{

void ShaderLoaderSystemProvider::OnCreateEntity(Entity entity)
{
    ShaderLoaderSystem::OnCreateEntity(entity);
}

void ShaderLoaderSystemProvider::OnDestroyEntity(Entity entity)
{
    ShaderLoaderSystem::OnDestroyEntity(entity);
}

std::shared_ptr<ShaderLoaderSystemProvider> ShaderLoaderSystemProvider::Get()
{
    return Engine::GetSystemProvider<ShaderLoaderSystemProvider>();
}
} // namespace Vultr
