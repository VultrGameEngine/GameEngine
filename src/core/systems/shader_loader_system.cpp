#include "../../../include/core/systems/shader_loader_system.h"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/ecs/coordinator/coordinator.hpp"
#include "../../../include/core/components/shader_component.h"

std::shared_ptr<ShaderLoaderSystem> ShaderLoaderSystem::instance = 0;
Signature ShaderLoaderSystem::signature = 0;

std::shared_ptr<ShaderLoaderSystem> ShaderLoaderSystem::Get()
{
    if (instance == 0)
        instance = RegisterSystem();
    return instance;
}
void ShaderLoaderSystem::DestroyEntity(Entity entity)
{
    auto &shader_component = Coordinator::Get()->GetComponent<ShaderComponent>(entity);
    if (shader_component.shader != 0)
    {
        glDeleteProgram(shader_component.shader);
    }
}

void ShaderLoaderSystem::InitShaders()
{
    // A list of existing, loaded shaders in memory
    std::unordered_map<std::string, unsigned int> path_to_shader_map{};

    // A list of entities that have a ShaderComponent, but have not loaded their shaders to the gpu yet
    std::set<Entity> entities_with_unloaded_shader{};
    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        auto &shader_component = Coordinator::Get()->GetComponent<ShaderComponent>(entity);

        // If the shader has been initialized, then we can add it to the path_to_shader_map
        if (shader_component.shader != 0)
        {
            // If the shader ID does not already exist
            if (path_to_shader_map.find(shader_component.shader_path) == path_to_shader_map.end())
            {
                // Add it to the map
                path_to_shader_map.insert({shader_component.shader_path, shader_component.shader});
            }
        }
        // If the shader id has not been initialized, that means that we have to load this shader
        else
        {
            entities_with_unloaded_shader.insert(entity);
        }
    }
    for (Entity entity_with_unloaded_shader : entities_with_unloaded_shader)
    {
        auto &shader_component = Coordinator::Get()->GetComponent<ShaderComponent>(entity_with_unloaded_shader);

        // If there is an existing shader with the same path and it has already loaded
        // we can just use that shader id in this entity
        std::unordered_map<std::string, unsigned int>::iterator it = path_to_shader_map.find(shader_component.shader_path);
        if (it != path_to_shader_map.end())
        {
            // Copy the existing shader id into this entity
            shader_component.shader = it->second;
        }
        // If there is no existing shader with that path, then we need to create it
        else
        {
            ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
            unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

            GLuint ViewProjection_location = glGetUniformLocation(shader, "ViewProjection");
            GLuint ViewMatrix = glGetUniformLocation(shader, "V");
            GLuint ModelMatrix = glGetUniformLocation(shader, "M");
            GLuint LightID = glGetUniformLocation(shader, "lightposition");
            shader_component.shader = shader;
            shader_component.ViewProjection_location = ViewProjection_location;
            shader_component.ViewMatrix = ViewMatrix;
            shader_component.ModelMatrix = ModelMatrix;
            shader_component.LightID = LightID;
        }
    }
}

std::shared_ptr<ShaderLoaderSystem> ShaderLoaderSystem::RegisterSystem()
{
    std::shared_ptr<ShaderLoaderSystem> ptr = Coordinator::Get()->RegisterSystem<ShaderLoaderSystem>();
    signature.set(Coordinator::Get()->GetComponentType<ShaderComponent>(), true);
    Coordinator::Get()->SetSystemSignature<ShaderLoaderSystem>(signature);
    return ptr;
}
