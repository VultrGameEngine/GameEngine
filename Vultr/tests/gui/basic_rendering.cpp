#include <gtest/gtest.h>
#include <gui/core/context.h>

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
        engine_update_game(vultr, lastTime, false);
        IMGUI::begin(c);
        auto mouse_pos = InputSystem::get_mouse_position();
        mouse_pos.y = 1 - mouse_pos.y;
        mouse_pos *= RenderSystem::get_dimensions(GAME);
        IMGUI::draw_rect(c, Vec4(255, 255, 255, 255), mouse_pos, Vec2(1000, 1000));
        IMGUI::draw_rect(c, Vec4(255, 0, 0, 255), mouse_pos + Vec2(250, 250), Vec2(500, 500));
        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}
