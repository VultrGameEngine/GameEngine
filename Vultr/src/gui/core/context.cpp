#include <gui/core/context.h>
#include <glm/gtc/type_ptr.hpp>

#include <core/system_providers/render_system_provider.h>

#include <core/system_providers/input_system_provider.h>
#include <helpers/font_importer.h>
#include <core/system_providers/font_system_provider.h>

using namespace Vultr;

void IMGUI::update_mouse_state(MouseState &m, Input::MouseButton button)
{
    auto state = InputSystem::get_mouse_button(button);
    if (m.state == state)
    {
        m.mouse_went_down = false;
        m.mouse_went_up = false;
    }
    else
    {
        if (!m.state && state)
        {
            m.mouse_went_down = true;
            m.mouse_went_up = false;
        }
        else if (m.state && !state)
        {
            m.mouse_went_up = true;
            m.mouse_went_down = false;
        }
    }
    m.state = state;
}

bool IMGUI::is_hot(Context *c, UI_ID id)
{
    return c->hot == id;
}
bool IMGUI::is_active(Context *c, UI_ID id)
{
    return c->active == id;
}

void IMGUI::set_hot(Context *c, UI_ID id)
{
    c->hot = id;
}

void IMGUI::set_not_hot(Context *c, UI_ID id)
{
    if (is_hot(c, id))
        c->hot = NO_ID;
}

void IMGUI::set_active(Context *c, UI_ID id)
{
    c->active = id;
}

void IMGUI::set_not_active(Context *c, UI_ID id)
{
    if (is_active(c, id))
        c->active = NO_ID;
}

IMGUI::Context *IMGUI::new_context(const IMGUI::Window &window)
{
    Context *context = new Context();
    context->window = window;
    context->renderer = new_imgui_renderer();
    auto default_font = FontSource("/home/brandon/Dev/Monopoly/res/fonts/Antonio-Bold.ttf");
    context->font = FontImporter::import_font(default_font, FontSystem::get_provider().library);
    return context;
}

void IMGUI::begin(Context *c, const UpdateTick &t)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    update_mouse_state(c->left_mb, Input::MOUSE_LEFT);
    update_mouse_state(c->right_mb, Input::MOUSE_RIGHT);
    update_mouse_state(c->middle_mb, Input::MOUSE_MIDDLE);

    c->delta_time = t.m_delta_time;
}

void IMGUI::end(Context *c)
{
}

bool IMGUI::mouse_over(Vec2 top_left, Vec2 size)
{
    auto mp = InputSystem::get_mouse_position();
    mp.y = 1 - mp.y;
    mp *= RenderSystem::get_dimensions(GAME);
    return mp.x > top_left.x && mp.x < top_left.x + size.x && mp.y > top_left.y && mp.y < top_left.y + size.y;
}

void IMGUI::draw_rect(Context *c, Vec4 color, Vec2 position, Vec2 dimensions, Shader *shader)
{
    auto col = gl_get_color(color);
    auto size = gl_get_size(dimensions);

    auto pos = gl_get_position(position, size);
    if (shader == nullptr)
    {
        shader = c->renderer.default_gui_shader;
        shader->Bind();
        shader->SetUniform4f("color", col);
    }

    glm::mat4 transform = glm::translate(Vec3(pos, 0)) * glm::scale(Vec3(size, 1));
    shader->SetUniformMatrix4fv("transform", glm::value_ptr(transform));

    c->renderer.quad->Draw();
}

void IMGUI::draw_texture(Context *c, Texture *tex, Vec2 position, Vec2 dimensions, Shader *shader)
{
    auto size = gl_get_size(dimensions);

    auto pos = gl_get_position(position, size);
    tex->Bind(GL_TEXTURE0);
    if (shader == nullptr)
    {
        shader = c->renderer.texture_gui_shader;
        shader->Bind();
        shader->SetUniform4f("color", Vec4(1));
        shader->SetUniform1i("tex", 0);
    }

    glm::mat4 transform = glm::translate(Vec3(pos, 0)) * glm::scale(Vec3(size, 1));
    shader->SetUniformMatrix4fv("transform", glm::value_ptr(transform));

    c->renderer.quad->Draw();
}

void IMGUI::draw_batch(Context *c, QuadBatch *batch, u32 quads)
{
    quad_batch_draw(batch, quads);
}
