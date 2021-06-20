#include <gtest/gtest.h>
#include <gui/core/context.h>
#include <gui/widgets/button.h>

#include <vultr.hpp>

using namespace Vultr;

TEST(IMGUI, Rect)
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

    float lastTime = 0;
    engine_init(vultr, false);

    auto window = IMGUI::new_window();
    auto *c = IMGUI::new_context(window);

    while (!InputSystem::get_key(Input::KEY_ESCAPE))
    {
        const auto &tick = engine_update_game(vultr, lastTime, false);
        IMGUI::begin(c, tick);
        auto mouse_pos = InputSystem::get_mouse_position();
        mouse_pos.y = 1 - mouse_pos.y;
        mouse_pos *= RenderSystem::get_dimensions(GAME);
        IMGUI::draw_rect(c, Vec4(255), Vec2(0), RenderSystem::get_dimensions(GAME));

        if (IMGUI::button(c, __LINE__, Color(Vec4(255, 0, 0, 255)), Vec2(500), Vec2(500)))
        {
            // break;
        }

        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}
