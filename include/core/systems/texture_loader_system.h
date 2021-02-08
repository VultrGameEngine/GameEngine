// TextureLoader loads in all the specified meshes in all of the entities that
// have a TextureComponent If there are entities that have a TextureComponent
// and do not have their buffers or opengl data set then TextureLoader will do
// that with the specified path

#pragma once
#include <core/components/sky_box_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <rendering/models/texture.h>

namespace Brick3D
{
class TextureLoaderSystem : public System
{
  public:
    static void Update();
    static void RegisterSystem();

  protected:
    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return TextureLoaderSystemProvider::Get();
    }
};
} // namespace Brick3D
