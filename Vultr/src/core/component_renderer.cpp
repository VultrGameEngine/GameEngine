#include <core/component_renderer.h>
#include <engine.hpp>

RenderMemberResult was_edited()
{
    RenderMemberResult res;
    if (ImGui::IsItemActivated())
    {
        res.started_editing = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        res.finished_editing = true;
    }
    return res;
}
