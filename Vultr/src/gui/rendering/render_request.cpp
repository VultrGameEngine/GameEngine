#include <gui/rendering/render_request.h>
#include <glm/gtc/type_ptr.hpp>
#include <gui/utils/opengl.h>
#include <core/system_providers/input_system_provider.h>
#include <core/system_providers/render_system_provider.h>
#include <gui/core/context.h>

using namespace Vultr;

bool IMGUI::RenderRequest::operator<(const RenderRequest &other) const
{
    return this->z_index < other.z_index;
}

void IMGUI::destroy_render_request(RenderRequest &r)
{
    delete r.material;
}
