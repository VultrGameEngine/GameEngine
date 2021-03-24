#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
#include <gui/rendering/gui_vertex.h>
#include <gui/rendering/gui_render_group.h>
#include <gui/framework/window.h>
#include <gui/framework/build_context.h>
#include <rendering/models/shader.h>
#include <helpers/shader_importer.h>

#define MAX_GROUPS 10

namespace Vultr
{
class GUISystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<GUISystemProvider> Get()
    {
        return World::GetSystemProvider<GUISystemProvider>();
    }
    GUI::Window *window;
    GUI::BuildContext *context;
    GUI::WindowElement *root_element;
    Shader *gui_shader;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};
} // namespace Vultr
