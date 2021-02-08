// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <core/models/update_tick.h>
#include <core/system_providers/render_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>
#include <rendering/renderer_3d.h>

namespace Brick3D
{
class RenderSystem : public System
{
  public:
    static void Update(UpdateTick meta_data);
    static void RegisterSystem();
    static void Resize(int width, int height, unsigned int type);

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return RenderSystemProvider::Get();
    }

    // Private helper methods
  private:
    static void RenderElements(unsigned int type,
                               TransformComponent camera_transform,
                               CameraComponent camera_component, Entity light);
    static void RenderSkybox(unsigned int type, TransformComponent camera_transform,
                             CameraComponent camera_component);
};
} // namespace Brick3D
