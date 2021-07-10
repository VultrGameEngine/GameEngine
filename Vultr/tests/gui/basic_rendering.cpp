#include <gtest/gtest.h>
#include <gui/core/context.h>
#include <gui/widgets/button.h>
#include <gui/widgets/image.h>
#include <gui/widgets/text.h>
#include <gui/widgets/center.h>
#include <gui/widgets/sized.h>
#include <gui/widgets/padding.h>
#include <gui/widgets/text_input.h>
#include <gui/materials/default_gui_material.h>
#include <gui/utils/opengl.h>
#include <helpers/texture_importer.h>

#include <vultr.hpp>
#include "basic_rendering_test.h"

using namespace Vultr;

void basic_rendering_test()
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

    auto resource_directory = Directory("/home/brandon/Dev/Monopoly/res");
    change_working_directory(resource_directory.path);

    float lastTime = 0;
    engine_init(vultr, false);

    auto window = IMGUI::new_window();
    auto *c = IMGUI::new_context(window);
    auto *texture = new Texture(GL_TEXTURE_2D);
    auto texture_source = TextureSource("/home/brandon/Dev/Monopoly/GameEngine/Vultr/tests/gui/troll.png");
    TextureImporter::import(texture_source, *texture);

    change_world(new_world(engine_global()->component_registry));

    Entity camera = create_entity(get_current_world());
    entity_add_component(camera, TransformComponent());
    entity_add_component(camera, CameraComponent());

    u32 count = 0;
    bool toggle_fps = true;

    Vec2 size = Vec2(1920, 1080);
    Vec2 position = Vec2(0, 0);
    auto gl_size = IMGUI::gl_get_size(size);
    auto gl_position = IMGUI::gl_get_position(position, gl_size);

    auto screen_size = IMGUI::screen_get_size_from_gl(gl_size);
    auto screen_position = IMGUI::screen_get_position_from_gl(gl_position, gl_size);

    std::string value = "";
    while (!InputSystem::get_key(Input::KEY_ESCAPE))
    {
        const auto &tick = engine_update_game(vultr, lastTime, false);
        IMGUI::begin(c, tick);
        auto mouse_pos = InputSystem::get_mouse_position();
        mouse_pos.y = 1 - mouse_pos.y;

        // FPS counter
        if (toggle_fps)
        {
            IMGUI::text(c, __LINE__, std::to_string(tick.m_delta_time * 1000) + " ms",
                        {
                            .font_color = Color(255),
                            .font_size = 9,
                            .line_spacing = 1,
                            .highlight_color = Color(0, 0, 255, 255),
                        });
        }

        // Center button
        IMGUI::begin_center(c, __LINE__);
        {
            IMGUI::begin_sized(c, __LINE__, {.size = Vec2(mouse_pos.y * RenderSystem::get_dimensions(GAME).x, 250)});
            {
                IMGUI::text(c, __LINE__, "joe mama chungus big alskdjfklasj", {.font_color = Color(255)});
                // IMGUI::image(c, __LINE__, texture);
                // IMGUI::text_input(c, __LINE__, value, {.padding_style = {.insets = IMGUI::edge_insets_all(20)}});
                // if (IMGUI::text_button(c, __LINE__, std::to_string(count)))
                // {
                //     count++;
                //     toggle_fps = !toggle_fps;
                // }
            }
            IMGUI::end_sized(c);
        }
        IMGUI::end_center(c);

        IMGUI::end(c);

        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}
