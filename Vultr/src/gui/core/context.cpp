#include <gui/core/context.h>
#include <glm/gtc/type_ptr.hpp>

#include <core/system_providers/render_system_provider.h>

#include <core/system_providers/input_system_provider.h>

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
    return context;
}

void IMGUI::begin(Context *c, const UpdateTick &t)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    auto pos = position / RenderSystem::get_dimensions(GAME) * Vec2(2) + Vec2(-1, 1);
    pos.y = 2 - pos.y;
    auto col = color / Vec4(255);
    auto size = dimensions / RenderSystem::get_dimensions(GAME);

    pos += size / Vec2(1, -1);
    if (shader == nullptr)
        shader = c->renderer.default_gui_shader;

    shader->Bind();

    glm::mat4 transform = glm::translate(Vec3(pos, 0)) * glm::scale(Vec3(size, 1));
    shader->SetUniformMatrix4fv("transform", glm::value_ptr(transform));
    shader->SetUniform4f("color", col);

    c->renderer.quad->Draw();
}
