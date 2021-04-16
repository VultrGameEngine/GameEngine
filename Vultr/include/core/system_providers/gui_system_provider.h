#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
#include <gui/rendering/gui_vertex.h>
#include <gui/rendering/gui_render_group.h>
#include <gui/framework/window.h>
#include <gui/framework/build_context.h>
#include <rendering/models/shader.h>
#include <helpers/shader_importer.h>
#include <engine.hpp>

#define MAX_GROUPS 10

namespace Vultr
{
class GUISystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<GUISystemProvider> Get()
    {
        return Engine::GetSystemProvider<GUISystemProvider>();
    }
    GUI::Window *window = nullptr;
    GUI::BuildContext *context = nullptr;
    GUI::WindowElement *root_element = nullptr;
    Shader *gui_shader = nullptr;
    template <class Archive> void serialize(Archive &ar)
    {
        // We pass this cast to the base type for each base type we
        // need to serialize.  Do this instead of calling serialize functions
        // directly
        ar(cereal::base_class<SystemProvider>(this));
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
VultrRegisterSystemProvider(Vultr::GUISystemProvider)
