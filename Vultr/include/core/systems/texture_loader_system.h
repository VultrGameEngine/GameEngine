// TextureLoader loads in all the specified meshes in all of the entities that
// have a TextureComponent If there are entities that have a TextureComponent
// and do not have their buffers or opengl data set then TextureLoader will do
// that with the specified path

#pragma once
#include <core/components/sky_box_component.h>
#include <core/system_providers/texture_loader_system_provider.h>
#include <glm/glm.hpp>
#include <memory>
#include <rendering/types/texture.h>

namespace Vultr::TextureLoaderSystem
{
    void register_system();
    void update();
    void on_create_entity(Entity entity);
    void load_texture(const MaterialComponent &mat);
} // namespace Vultr::TextureLoaderSystem
