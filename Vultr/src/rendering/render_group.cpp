#include <ecs/world/world.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render_group.h>

namespace Vultr
{
    // void RenderGroup::Render(RenderContext context, RenderType type)
    // {

    //     // Bind the shader that will be used for rendering all the following meshes
    //     bind_shader(this->shader);

    //     // Set the view matrix and projection matrix, as they do not change between
    //     // meshes
    //     // this->shader->SetUniformMatrix4fv("view", glm::value_ptr(context.camera_transform.GetViewMatrix()));

    //     // this->shader->SetUniformMatrix4fv("projection", glm::value_ptr(context.camera_component.GetProjectionMatrix(context.dimensions.x, context.dimensions.y)));

    //     // Render all registered entities
    //     for (RenderEntity entity : entities)
    //     {
    //         Mesh *mesh = entity.mesh;
    //         if (type == Deferred)
    //         {
    //             // // Bind the entity's texture
    //             // LoadedTexture *diffuse = entity.GetDiffuse();
    //             // if (diffuse != nullptr)
    //             // {
    //             //     entity.GetDiffuse()->Bind(GL_TEXTURE0);
    //             //     this->shader->SetUniform1i("material.diffuse", 0);
    //             // }
    //             // LoadedTexture *specular = entity.GetSpecular();
    //             // if (specular != nullptr)
    //             // {
    //             //     entity.GetSpecular()->Bind(GL_TEXTURE1);
    //             //     this->shader->SetUniform1i("material.specular", 1);
    //             // }
    //             // this->shader->SetUniform3f("lightPos", context.light_position);
    //             // this->shader->SetUniform3f("objectColor",
    //             // Vec3(1.0f, 1.0f, 1.0f)); this->shader->SetUniform3f("viewPos",
    //             // context.camera_transform.position);
    //         }

    //         // TransformComponent &transform_component =
    //         //     World::GetComponent<TransformComponent>(entity.entity);

    //         // this->shader->SetUniformMatrix4fv(
    //         //     "model", glm::value_ptr(transform_component.Matrix()));

    //         // this->shader->SetUniform3f("lightColor", Vec3(1.0f, 1.0f, 1.0f));

    //         // // Bind the vao and ibo
    //         // glBindVertexArray(loaded_mesh->vao);
    //         // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loaded_mesh->ibo);

    //         // // Draw the triangles
    //         // glDrawElements(GL_TRIANGLES, loaded_mesh->indices.size(),
    //         // GL_UNSIGNED_SHORT,
    //         //                (void *)0);
    //     }
    // }

} // namespace Vultr
