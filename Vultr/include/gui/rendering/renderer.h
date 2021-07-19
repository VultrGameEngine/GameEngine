#pragma once
#include <rendering/types/mesh.h>
#include <rendering/types/shader.h>

namespace Vultr
{
    namespace IMGUI
    {
        struct Renderer
        {
            Mesh *quad;

            Shader default_gui_shader;
            Shader texture_gui_shader;
            Shader batch_gui_shader;
            Shader rounded_gui_shader;
        };

        Renderer new_imgui_renderer();
        void destroy_imgui_renderer(const Renderer &r);
    } // namespace IMGUI
} // namespace Vultr
