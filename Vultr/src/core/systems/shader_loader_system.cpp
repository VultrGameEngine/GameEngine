#include <core/components/material_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <ecs/component/component.hpp>
#include <ecs/world/world.hpp>
#include <helpers/shader_importer.h>

namespace Vultr
{

void ShaderLoaderSystem::OnCreateEntity(Entity entity)
{
    ShaderLoaderSystemProvider &provider = ShaderLoaderSystemProvider::Get();
    auto &material_component = World::GetComponent<MaterialComponent>(entity);
    // If we have already loaded the shader and cached it, then reuse the id and
    // don't reload
    if (material_component.GetShader() != nullptr)
        return;

    // If we haven't cached this shader, load it and save it in the loaded shaders
    // Create the shader on the gpu
    unsigned int shader_id =
        ShaderImporter::CreateShader(material_component.shader_path);

    // Create the shader wrapper with the given shader id
    Shader *shader = new Shader(shader_id, material_component.GetRenderType());

    // Add it to the loaded shaders
    provider.loaded_shaders[material_component.shader_path] = shader;
}

void ShaderLoaderSystem::OnDestroyEntity(Entity entity)
{
    // auto &shader_component = World::GetComponent<MaterialComponent>(entity);
    // Shader *shader = shader_component.GetShader();
    // if (shader != nullptr)
    // {
    //     delete shader;
    // }
}

void ShaderLoaderSystem::RegisterSystem()
{
    ShaderLoaderSystemProvider &provider = ShaderLoaderSystemProvider::Get();
    provider.signature.set(World::GetComponentType<MaterialComponent>(), true);
    World::RegisterSystem<ShaderLoaderSystem>(provider.signature);
}
} // namespace Vultr
