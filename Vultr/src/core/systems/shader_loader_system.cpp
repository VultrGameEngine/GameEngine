#include <core/components/material_component.h>
#include <core/system_providers/shader_loader_system_provider.h>
#include <core/systems/shader_loader_system.h>
#include <ecs/component/component.hpp>
#include <ecs/world/world.hpp>
#include <helpers/shader_importer.h>
#include <engine.hpp>

namespace Vultr
{

void ShaderLoaderSystem::Update()
{
    ShaderLoaderSystemProvider &provider = *ShaderLoaderSystemProvider::Get();
    for (Entity entity : provider.entities)
    {
        CheckAndLoadShader(entity);
    }
}

void ShaderLoaderSystem::OnCreateEntity(Entity entity)
{
    CheckAndLoadShader(entity);
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

void ShaderLoaderSystem::LoadShader(const MaterialComponent &mat)
{
    ShaderLoaderSystemProvider &provider = *ShaderLoaderSystemProvider::Get();
    // If we have already loaded the shader and cached it, then reuse the id and
    // don't reload
    Shader *material_shader = ShaderLoaderSystemProvider::GetShader(mat.shader_path);
    if (material_shader != nullptr)
        return;

    // If we haven't cached this shader, load it and save it in the loaded shaders
    // Create the shader on the gpu
    // unsigned int shader_id =
    //     ShaderImporter::CreateShader(material_component.shader_path);
    Shader *shader = ShaderImporter::ImportShader(mat.shader_path);

    // Create the shader wrapper with the given shader id
    // Shader *shader = new Shader(shader_id, Forward);

    // Add it to the loaded shaders
    provider.loaded_shaders[mat.shader_path] = shader;
}

void ShaderLoaderSystem::CheckAndLoadShader(Entity entity)
{
    auto &material_component = entity.GetComponent<MaterialComponent>();
    LoadShader(material_component);
}

void ShaderLoaderSystem::RegisterSystem()
{
    Signature signature;
    signature.set(
        Engine::GetComponentRegistry().GetComponentType<MaterialComponent>(), true);
    Engine::RegisterGlobalSystem<ShaderLoaderSystemProvider>(signature);
}
} // namespace Vultr
