#include <gtest/gtest.h>
#include <gui/core/context.h>
#include <gui/widgets/button.h>
#include <gui/widgets/image.h>
#include <gui/widgets/text.h>
#include <gui/widgets/center.h>
#include <gui/widgets/align.h>
#include <gui/widgets/sized.h>
#include <gui/widgets/stack.h>
#include <gui/widgets/row.h>
#include <gui/widgets/constrained_box.h>
#include <gui/widgets/padding.h>
#include <gui/widgets/text_input.h>
#include <gui/widgets/container.h>
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
    bool toggle_fps = false;

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

        // Begin IMGUI
        IMGUI::begin(c, tick);

        // Mouse pos stuff for testing
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
        IMGUI::begin_align(c, __LINE__, IMGUI::Alignment::CENTER());
        {
            // IMGUI::begin_constrained_box(c, __LINE__, {.max_width = mouse_pos.y * RenderSystem::get_dimensions(GAME).x});
            // {
            //     IMGUI::text_input(c, __LINE__, value,
            //                       {
            //                           .padding_style =
            //                               {
            //                                   .insets = IMGUI::EdgeInsets::ALL(20),
            //                               },
            //                       });
            //     // if (IMGUI::text_button(c, __LINE__, std::to_string(count)))
            //     // {
            //     //     count++;
            //     //     toggle_fps = !toggle_fps;
            //     // }
            // }
            // IMGUI::end_constrained_box(c);
            IMGUI::begin_row(c, __LINE__,
                             {
                                 .main_axis_alignment = IMGUI::MainAxisAlignment::SPACE_EVENLY,
                                 .cross_axis_alignment = IMGUI::CrossAxisAlignment::CENTER,
                             });
            {
                Color colors[5] = {Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255), Color(255), Color(255, 0, 0)};
                for (u32 i = 0; i < 3; i++)
                {
                    IMGUI::begin_row_element(c, i);
                    {
                        IMGUI::begin_sized(c, IMGUI::ui_id("RowElementSized") + i, Vec2(500));
                        {
                            IMGUI::container(c, IMGUI::ui_id("RowElementContainer") + i, {.color = colors[i]});
                        }
                        IMGUI::end_sized(c);
                    }
                    IMGUI::end_row_element(c);
                }
            }
            IMGUI::end_row(c);
        }
        IMGUI::end_align(c);

        IMGUI::end(c);

        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}
