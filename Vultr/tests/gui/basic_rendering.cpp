#include <gtest/gtest.h>
#include <gui/core/context.h>
#include <gui/widgets/button.h>
#include <gui/widgets/image.h>
#include <gui/widgets/text.h>
#include <helpers/texture_importer.h>

#include <vultr.hpp>

using namespace Vultr;

TEST(IMGUI, Rect)
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

    while (!InputSystem::get_key(Input::KEY_ESCAPE))
    {
        const auto &tick = engine_update_game(vultr, lastTime, false);
        IMGUI::begin(c, tick);
        auto mouse_pos = InputSystem::get_mouse_position();
        mouse_pos.y = 1 - mouse_pos.y;
        mouse_pos *= RenderSystem::get_dimensions(GAME);
        IMGUI::draw_rect(c, Vec4(255), Vec2(0), RenderSystem::get_dimensions(GAME));
        IMGUI::text(c, __LINE__, std::to_string(tick.m_delta_time * 1000) + " ms", Vec2(0),
                    {
                        .highlight_color = Color(0, 0, 255, 255),
                        .color = Color(255),
                    });

        IMGUI::image(c, __LINE__, texture, Vec2(1000), Vec2(200));

        if (IMGUI::button(c, __LINE__, Color(Vec4(255, 0, 0, 255)), Vec2(500), Vec2(500)))
        {
        }
        IMGUI::text(c, __LINE__, "#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", Vec2(500),
                    {
                        .highlight_color = Color(0, 255),
                        .color = Color(255),
                    });

        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}
