#include <gui/rendering/gui_vertex.h>

namespace Vultr
{
template <> void VertexArray::Setup<GUI::GUIVertex>()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUI::GUIVertex),
                          (const void *)offsetof(GUI::GUIVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GUI::GUIVertex),
                          (const void *)offsetof(GUI::GUIVertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GUI::GUIVertex),
                          (const void *)offsetof(GUI::GUIVertex, texture));
}
} // namespace Vultr
