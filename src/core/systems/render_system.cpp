#include "../../../include/core/systems/render_system.h"
#include "../../../include/core/systems/mesh_loader_system.h"
#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/world/world.hpp"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/core/components/static_mesh_component.h"
#include "../../../include/core/components/texture_component.h"
#include "../../../include/core/components/transform_component.h"
#include "../../../include/core/components/shader_component.h"
#include "../../../include/core/components/camera_component.h"
#include "../../../include/core/components/controller_component.h"
#include "../../../include/core/components/static_mesh_cache.h"
#include "../../../include/core/components/texture_cache.h"
#include "../../../include/core/components/light_component.h"
#include "../../../include/helpers/loading_state.h"
#include "../../../include/core/components/sky_box_component.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

std::shared_ptr<RenderSystem> RenderSystem::Get()
{
    static std::shared_ptr<RenderSystem> instance;
    if (instance == 0)
    {
        instance = RenderSystem::RegisterSystem();
    }
    return instance;
}
void RenderSystem::DestroyEntity(Entity entity)
{
    auto &static_mesh_component = World::GetComponent<StaticMeshComponent>(entity);
    glDeleteBuffers(1, &static_mesh_component.vbo);
    glDeleteBuffers(1, &static_mesh_component.nbo);
    glDeleteBuffers(1, &static_mesh_component.ibo);
    glDeleteVertexArrays(1, &static_mesh_component.vao);
}

// Used in the actual update loop in main
void RenderSystem::Update(float delta_time)
{
    Signature camera_signature;
    camera_signature.set(World::GetComponentType<CameraComponent>(), true);
    camera_signature.set(World::GetComponentType<TransformComponent>(), true);
    Entity camera = -1;
    for (Entity entity : World::GetEntities(camera_signature))
    {
        auto &camera_component = World::GetComponent<CameraComponent>(entity);
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        if (camera_component.enabled)
        {
            camera_component.view_matrix = glm::lookAt(transform_component.position, transform_component.position + transform_component.Forward(), glm::vec3(0, 1, 0));
            camera = entity;
            break;
        }
    }

    // If no camera is in the scene, then something is wrong and we can't render
    if (camera == -1)
    {
        std::cout << "NO CAMERA FOUND" << std::endl;
        return;
    }
    auto &camera_transform = World::GetComponent<TransformComponent>(camera);
    auto &camera_component = World::GetComponent<CameraComponent>(camera);

    glm::vec3 light_position = glm::vec3(0, 0, 0);
    Entity light;
    Signature light_signature;
    light_signature.set(World::GetComponentType<LightComponent>(), true);
    camera_signature.set(World::GetComponentType<TransformComponent>(), true);
    for (Entity entity : World::GetEntities(light_signature))
    {
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        light = entity;
        light_position = transform_component.position;
        break;
    }

    glDepthFunc(GL_LEQUAL); // Ensure depth test passes when values are equal to the depth buffer's content
    glm::mat4(glm::mat3(camera_component.view_matrix));
    Signature skybox_signature;
    skybox_signature.set(World::GetComponentType<SkyBoxComponent>(), true);
    skybox_signature.set(World::GetComponentType<ShaderComponent>(), true);

    // If the camera has a texture, that means it has a skybox
    if ((World::GetSignature(camera) & skybox_signature) == skybox_signature)
    {
        auto &shader_component = World::GetComponent<ShaderComponent>(camera);
        auto &skybox_component = World::GetComponent<SkyBoxComponent>(camera);
        LoadedTexture *texture = TextureLoaderSystem::Get()->GetTexture(skybox_component.identifier);
        // If we have loaded the bytes into memory
        if (texture != nullptr && texture->loaded == loaded)
        {
            glDepthMask(GL_FALSE);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

            glUseProgram(shader_component.shader);
            unsigned int SkyBox = glGetUniformLocation(shader_component.shader, "skybox");
            glUniform1i(SkyBox, 0);
            glUniformMatrix4fv(shader_component.View, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera_component.view_matrix))));
            glUniformMatrix4fv(shader_component.Projection, 1, GL_FALSE, glm::value_ptr(camera_component.GetProjectionMatrix()));

            if (skybox_component.vbo == 0 || skybox_component.vao == 0)
            {
                glGenVertexArrays(1, &skybox_component.vao);
                glGenBuffers(1, &skybox_component.vbo);
                glBindVertexArray(skybox_component.vao);
                glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * skybox_component.GetVertices().size(), &(skybox_component.GetVertices()[0]), GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }

            glBindVertexArray(skybox_component.vao);
            glBindBuffer(GL_ARRAY_BUFFER, skybox_component.vbo);
            glDrawArrays(GL_TRIANGLES, 0, skybox_component.GetVertices().size());
            glDepthMask(GL_TRUE);
        }
    }
    glDepthFunc(GL_LESS); // Reset depth test

    Signature signature;
    signature.set(World::GetComponentType<StaticMeshComponent>(), true);
    signature.set(World::GetComponentType<TransformComponent>(), true);
    signature.set(World::GetComponentType<ShaderComponent>(), true);

    for (Entity entity : World::GetEntities(signature))
    {
        auto &static_mesh_component = World::GetComponent<StaticMeshComponent>(entity);
        auto &transform_component = World::GetComponent<TransformComponent>(entity);
        auto &shader_component = World::GetComponent<ShaderComponent>(entity);

        if (shader_component.shader == 0)
        {

            std::cout << "Entity " << entity << " encountered an error: "
                      << "Shader not found!" << std::endl;
            continue;
        }
        LoadedStaticMesh *mesh = MeshLoaderSystem::GetMesh(static_mesh_component.path);
        if (mesh == nullptr || mesh->loaded != loaded)
            continue;

        // If the static_mesh_component does not have a vao, it means that it has not been bound to a buffer yet
        // We will do that here
        if (static_mesh_component.vao == 0)
        {
            // Create the vertex array object
            glGenVertexArrays(1, &static_mesh_component.vao);

            // Bind the vertex array
            glBindVertexArray(static_mesh_component.vao);
        }

        // If the static_mesh_component does not have a vbo, it means that it has not been bound to a buffer yet
        // We will do that here
        if (static_mesh_component.vbo == 0)
        {
            // Create the vertex buffer
            glGenBuffers(1, &static_mesh_component.vbo);

            // Bind the vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, static_mesh_component.vbo);

            // Set the buffer data for the vertex buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->vertices.size(), &(mesh->vertices[0]), GL_STATIC_DRAW);

            // Enable vertex data
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }

        // If the static_mesh_component does not have a nbo, it means that it has not been bound to a buffer yet
        // We will do that here
        if (static_mesh_component.nbo == 0)
        {
            // Create the normal buffer
            glGenBuffers(1, &static_mesh_component.nbo);

            // Bind the normal buffer
            glBindBuffer(GL_ARRAY_BUFFER, static_mesh_component.nbo);

            // Set the buffer data for the normal buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->normals.size(), &(mesh->normals[0]), GL_STATIC_DRAW);

            // Enable normal data
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
        // If the static_mesh_component does not have a uvbo, it means that it has not been bound to a buffer yet
        // We will do that here
        if (static_mesh_component.uvbo == 0)
        {
            // Create the UV buffer
            glGenBuffers(1, &static_mesh_component.uvbo);

            // Bind the UV buffer
            glBindBuffer(GL_ARRAY_BUFFER, static_mesh_component.uvbo);

            // Set the buffer data for the UV buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->uvs.size(), &(mesh->uvs[0]), GL_STATIC_DRAW);

            // Enable normal data
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }

        // If the static_mesh_component does not have a ibo, it means that it has not been bound to a buffer yet
        // We will do that here
        if (static_mesh_component.ibo == 0)
        {
            // Create the index buffer
            glGenBuffers(1, &static_mesh_component.ibo);

            // Bind the index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_mesh_component.ibo);

            // Set the buffer data for the index buffer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->indices.size(), &(mesh->indices[0]), GL_STATIC_DRAW);
        }

        glUseProgram(shader_component.shader);
        glUniformMatrix4fv(shader_component.Model, 1, GL_FALSE, glm::value_ptr(transform_component.Matrix()));
        glUniformMatrix4fv(shader_component.View, 1, GL_FALSE, glm::value_ptr(camera_component.view_matrix));
        glUniformMatrix4fv(shader_component.Projection, 1, GL_FALSE, glm::value_ptr(camera_component.GetProjectionMatrix()));
        glUniform3f(shader_component.LightPosition, light_position.x, light_position.y, light_position.z);
        glUniform3f(shader_component.LightColor, 1.0f, 1.0f, 1.0f);
        glUniform3f(shader_component.ObjectColor, 1.0f, 1.0f, 1.0f);
        glUniform3f(shader_component.ViewPosition, camera_transform.position.x, camera_transform.position.y, camera_transform.position.z);

        // If there is a texture attached to an entity
        Signature texture_signature;
        texture_signature.set(World::GetComponentType<TextureComponent>(), true);
        if ((World::GetSignature(entity) & texture_signature) == texture_signature)
        {
            auto &texture_component = World::GetComponent<TextureComponent>(entity);
            LoadedTexture *texture = TextureLoaderSystem::Get()->GetTexture(texture_component.path);
            // If we have loaded the bytes into memory
            if (texture != nullptr && texture->loaded == loaded)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture->id);
                glUniform1i(shader_component.TextureSampler, 0);
            }
        }

        glBindVertexArray(static_mesh_component.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_mesh_component.ibo);
        glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(unsigned short), GL_UNSIGNED_SHORT, (void *)0);
    }
}

// Internal private helper methods
std::shared_ptr<RenderSystem> RenderSystem::RegisterSystem()
{
    std::shared_ptr<RenderSystem> ptr = World::RegisterSystem<RenderSystem>();
    return ptr;
}
