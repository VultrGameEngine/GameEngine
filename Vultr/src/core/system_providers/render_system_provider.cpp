#include <core/system_providers/render_system_provider.h>
#include <engine.hpp>

namespace Vultr::RenderSystem
{
    Component &get_provider(Engine *e)
    {
        return *get_global_system_provider<Component>(e);
    }

    Vec2 get_dimensions(Engine *e, u8 type)
    {
        assert((type == GAME || type == SCENE) && "Please call this method with a valid type, either GAME or SCENE");

        auto &p = get_provider(e);

        if (type == GAME)
        {
            return p.game.dimensions;
        }
        else
        {
            return p.scene.dimensions;
        }
    }

    Entity get_entity_at_pixel(Engine *e, s32 x, s32 y)
    {
        auto &p = get_provider(e);
        auto &input = p.input_data;
        bind_framebuffer(&input.render_fbo);
        unsigned char data[4];
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        Entity pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;

        unbind_all_renderbuffers();

        return pickedID;
    }

} // namespace Vultr::RenderSystem
