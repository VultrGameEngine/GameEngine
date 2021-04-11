// TextureLoader loads in all the specified meshes in all of the entities that
// have a TextureComponent If there are entities that have a TextureComponent
// and do not have their buffers or opengl data set then TextureLoader will do
// that with the specified path

#pragma once
#include <core/components/sky_box_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <rendering/models/texture.h>

namespace Vultr
{
class TextureLoaderSystem
{
  public:
    static void LoadTexture(const MaterialComponent &mat);
    static void RegisterSystem();
    static void Update();
    static void OnCreateEntity(Entity entity);
    static SystemProvider &GetProvider()
    {
        std::shared_ptr<TextureLoaderSystemProvider> provider =
            TextureLoaderSystemProvider::Get();
        return *provider;
    }

  protected:
    static void CheckAndLoadTexture(Entity entity);
};
} // namespace Vultr
