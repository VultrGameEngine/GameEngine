#include <gui/rendering/stencil_request.h>

using namespace Vultr;

void IMGUI::destroy_stencil_request(StencilRequest *request)
{
    free(request);
}
