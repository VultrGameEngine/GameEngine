// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <core/models/update_tick.h>
#include <core/system_providers/render_system_provider.h>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>

namespace Vultr
{
class RenderSystem
{
  public:
    static void Update(UpdateTick meta_data);
    static void RegisterSystem();

    // Private helper methods
  private:
    static void RenderElements(unsigned int type);
    static void RenderSkybox(unsigned int type);
    static SystemProvider &GetProvider()
    {
        std::shared_ptr<RenderSystemProvider> provider = RenderSystemProvider::Get();
        return *provider;
    }
};
} // namespace Vultr
