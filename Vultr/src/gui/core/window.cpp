#include <core/system_providers/render_system_provider.h>
#include <gui/core/window.h>

using namespace Vultr;

IMGUI::Window IMGUI::new_window()
{
    auto dimensions = Vultr::RenderSystem::get_dimensions(GAME);

    Window window;

    window.dimensions = dimensions;
    window.position = Vec2(0, 0);
    return window;
}
