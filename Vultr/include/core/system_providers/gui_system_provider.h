#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/types/shader.h>
#include <helpers/shader_importer.h>
#include <engine.hpp>
#include <input/input.h>

#define MAX_GROUPS 10

namespace Vultr
{
    namespace GUISystem
    {
        struct Component : public SystemProvider
        {
            Shader *gui_shader = nullptr;
        };

        Component &get_provider(Engine *e);
    } // namespace GUISystem

    template <>
    inline const char *get_struct_name<GUISystem::Component>()
    {
        return "GUISystem";
    }
} // namespace Vultr
