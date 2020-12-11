#include "../../../include/core/systems/render_system.h"
#include "../../../include/core/systems/mesh_loader_system.h"
#include "../../../include/ecs/coordinator/coordinator.hpp"
#include "../../../include/ecs/component/component.hpp"
#include "../../../include/core/components/static_mesh_component.h"
#include "../../../include/core/components/transform_component.h"
#include "../../../include/core/components/shader_component.h"
#include "../../../include/core/components/camera_component.h"
#include "../../../include/core/components/controller_component.h"
#include "../../../include/core/components/static_mesh_cache.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

std::shared_ptr<RenderSystem> RenderSystem::instance = 0;

std::shared_ptr<RenderSystem> RenderSystem::Get()
{
    if (instance == 0)
    {
        instance = RenderSystem::RegisterSystem();
    }
    return instance;
}
void RenderSystem::DestroyEntity(Entity entity)
{
    auto &static_mesh_component = Coordinator::Get()->GetComponent<StaticMeshComponent>(entity);
    glDeleteBuffers(1, &static_mesh_component.vbo);
    glDeleteBuffers(1, &static_mesh_component.nbo);
    glDeleteBuffers(1, &static_mesh_component.ibo);
    glDeleteVertexArrays(1, &static_mesh_component.vao);
}

// Used in the actual update loop in main
void RenderSystem::Update(float delta_time)
{
    Signature camera_signature;
    camera_signature.set(Coordinator::Get()->GetComponentType<CameraComponent>(), true);
    camera_signature.set(Coordinator::Get()->GetComponentType<TransformComponent>(), true);
    Entity camera = -1;
    for (Entity entity : Coordinator::Get()->GetEntities(camera_signature))
    {
        auto &camera_component = Coordinator::Get()->GetComponent<CameraComponent>(entity);
        auto &transform_component = Coordinator::Get()->GetComponent<TransformComponent>(entity);
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

    Signature signature;
    signature.set(Coordinator::Get()->GetComponentType<StaticMeshComponent>(), true);
    signature.set(Coordinator::Get()->GetComponentType<TransformComponent>(), true);
    signature.set(Coordinator::Get()->GetComponentType<ShaderComponent>(), true);

    for (Entity entity : Coordinator::Get()->GetEntities(signature))
    {
        auto &static_mesh_component = Coordinator::Get()->GetComponent<StaticMeshComponent>(entity);
        auto &transform_component = Coordinator::Get()->GetComponent<TransformComponent>(entity);
        auto &shader_component = Coordinator::Get()->GetComponent<ShaderComponent>(entity);
        auto &camera_component = Coordinator::Get()->GetComponent<CameraComponent>(camera);
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
        glm::mat4 ProjectionViewModelMatrix = camera_component.GetProjectionMatrix() * camera_component.view_matrix * glm::translate(transform_component.position);

        glUseProgram(shader_component.shader);
        glUniformMatrix4fv(shader_component.ViewProjection_location, 1, GL_FALSE, &ProjectionViewModelMatrix[0][0]);
        glUniformMatrix4fv(shader_component.ModelMatrix, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
        glUniformMatrix4fv(shader_component.ViewMatrix, 1, GL_FALSE, &camera_component.view_matrix[0][0]);
        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(shader_component.LightID, lightPos.x, lightPos.y, lightPos.z);

        glBindVertexArray(static_mesh_component.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_mesh_component.ibo);
        glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(unsigned short), GL_UNSIGNED_SHORT, (void *)0);
    }
}

// Internal private helper methods
std::shared_ptr<RenderSystem> RenderSystem::RegisterSystem()
{
    std::shared_ptr<RenderSystem> ptr = Coordinator::Get()->RegisterSystem<RenderSystem>();
    return ptr;
}
