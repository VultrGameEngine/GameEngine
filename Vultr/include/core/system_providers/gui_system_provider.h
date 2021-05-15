#pragma once
#include <ecs/system/system_provider.hpp>
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
    namespace GUISystem
    {
        struct Component : public SystemProvider
        {
            GUI::Window *window = nullptr;
            GUI::BuildContext *context = nullptr;
            GUI::WindowElement *root_element = nullptr;
            Shader *gui_shader = nullptr;
        };

        Component &get_provider();
    } // namespace GUISystem

    template <>
    inline const char *get_struct_name<GUISystem::Component>()
    {
        return "GUISystem";
    }
} // namespace Vultr
