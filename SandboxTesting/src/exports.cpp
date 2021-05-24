#include <sandbox_test.h>
#include <exports.h>
#include <imgui/imgui.h>
void *init(void *engine)
{
    return new SandboxTest((Vultr::Engine *)engine);
}
void flush(void *game)
{
    delete game;
}
//void imgui_set_context(void *context)
//{
//    ImGui::SetCurrentContext((ImGuiContext *)context);
//}
