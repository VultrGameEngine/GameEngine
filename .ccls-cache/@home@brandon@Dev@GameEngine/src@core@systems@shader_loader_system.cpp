#include "../../../include/core/systems/shader_loader_system.h"
#include "../../../include/core/components/shader_component.h"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/ecs/world/world.hpp"

namespace Brick3D
{

void ShaderLoaderSystem::OnCreateEntity(Entity entity)
{
    // auto &shader_component = World::GetComponent<ShaderComponent>(entity);
    // // If we have already loaded the shader and cached it, then reuse the id and
    // // don't reload
    // if (loaded_shaders.find(shader_component.path) != loaded_shaders.end())
    //     return;

    // // If we haven't cached this shader, load it and save it in the loaded shaders
    // // Create the shader on the gpu
    // unsigned int shader_id = CreateShader(shader_component.path);

    // // Create the shader wrapper with the given shader id
    // Shader *shader = new Shader(shader_id);

    // // Add it to the loaded shaders
    // loaded_shaders[shader_component.path] = shader;
}

void ShaderLoaderSystem::OnDestroyEntity(Entity entity)
{
    // auto &shader_component = World::GetComponent<ShaderComponent>(entity);
    // Shader *shader = shader_component.GetShader();
    // if (shader != nullptr)
    // {
    //     shader->Delete();
    // }
}

void ShaderLoaderSystem::RegisterSystem()
{
    ShaderLoaderSystemProvider &provider = ShaderLoaderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<ShaderComponent>(), true);
    World::RegisterSystem<ShaderLoaderSystem>(provider.signature);
}
} // namespace Brick3D
