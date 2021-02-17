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

namespace Vultr
{
class TextureLoaderSystem : public System
{
  public:
    static void RegisterSystem();
    static void Update();

  protected:
    static void CheckAndLoadTexture(Entity entity);

    void OnCreateEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return TextureLoaderSystemProvider::Get();
    }
};
} // namespace Vultr
