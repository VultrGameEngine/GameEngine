#include <gui/rendering/gui_vertex.h>

namespace Vultr
{
    template <>
    void VertexArray::Setup<IMGUI::GUIVertex>()
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(IMGUI::GUIVertex), (const void *)offsetof(IMGUI::GUIVertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(IMGUI::GUIVertex), (const void *)offsetof(IMGUI::GUIVertex, color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(IMGUI::GUIVertex), (const void *)offsetof(IMGUI::GUIVertex, uv));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(IMGUI::GUIVertex), (const void *)offsetof(IMGUI::GUIVertex, texture));
    }
} // namespace Vultr
