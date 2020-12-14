#include "../../../include/core/systems/shader_loader_system.h"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/ecs/coordinator/coordinator.hpp"
#include "../../../include/core/components/shader_component.h"

std::shared_ptr<ShaderLoaderSystem> ShaderLoaderSystem::Get()
{
    static std::shared_ptr<ShaderLoaderSystem> instance;
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
    Signature signature;
    signature.set(Coordinator::Get()->GetComponentType<ShaderComponent>(), true);
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
            ShaderProgramSource source = ParseShader(shader_component.shader_path);
            unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

            GLuint Model = glGetUniformLocation(shader, "model");
            GLuint View = glGetUniformLocation(shader, "view");
            GLuint Projection = glGetUniformLocation(shader, "projection");
            GLuint LightPosition = glGetUniformLocation(shader, "lightPos");
            GLuint ViewPosition = glGetUniformLocation(shader, "viewPos");
            GLuint ObjectColor = glGetUniformLocation(shader, "objectColor");
            GLuint LightColor = glGetUniformLocation(shader, "lightColor");
            GLuint TextureSampler = glGetUniformLocation(shader, "textureSampler");
            shader_component.shader = shader;
            shader_component.Model = Model;
            shader_component.View = View;
            shader_component.Projection = Projection;
            shader_component.LightPosition = LightPosition;
            shader_component.ViewPosition = ViewPosition;
            shader_component.ObjectColor = ObjectColor;
            shader_component.LightColor = LightColor;
            shader_component.TextureSampler = TextureSampler;
        }
    }
}

std::shared_ptr<ShaderLoaderSystem> ShaderLoaderSystem::RegisterSystem()
{
    std::shared_ptr<ShaderLoaderSystem> ptr = Coordinator::Get()->RegisterSystem<ShaderLoaderSystem>();
    return ptr;
}
