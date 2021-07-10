#include <gui/core/context.h>
#include <glm/gtc/type_ptr.hpp>

#include <core/system_providers/render_system_provider.h>

#include <core/system_providers/input_system_provider.h>
#include <helpers/font_importer.h>
#include <core/system_providers/font_system_provider.h>
#include <gui/materials/default_gui_material.h>
#include <gui/utils/opengl.h>

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
    auto default_font = FontSource("/home/brandon/Dev/Monopoly/res/fonts/Roboto-Regular.ttf");
    context->font = FontImporter::import_font(default_font, FontSystem::get_provider().library);
    context->requests.reserve(512);
    return context;
}

void IMGUI::begin(Context *c, const UpdateTick &t)
{
    auto dimensions = RenderSystem::get_dimensions(GAME);
    glViewport(0, 0, dimensions.x, dimensions.y);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SCISSOR_TEST);

    update_mouse_state(c->left_mb, Input::MOUSE_LEFT);
    update_mouse_state(c->right_mb, Input::MOUSE_RIGHT);
    update_mouse_state(c->middle_mb, Input::MOUSE_MIDDLE);

    c->delta_time = t.m_delta_time;
    c->z_index = 0;
    c->drawing_id = NO_ID;
    c->index = std::stack<s32>();
    c->index.push(0);
}

void IMGUI::widget_accessed(Context *c, UI_ID id)
{
    c->widget_ids_to_be_removed.erase(id);
}

void IMGUI::end(Context *c)
{
    std::sort(c->requests.begin(), c->requests.end());
    c->widget_transforms.clear();

    // If there was an ID there before that did not have its state accessed this frame, then we should clean up the cache
    if (c->widget_ids_to_be_removed.size() > 0)
    {
        for (auto id : c->widget_ids_to_be_removed)
        {
            for (auto &[_, array] : c->cache_arrays)
            {
                array->widget_destroyed(id);
            }
        }
    }
    c->widget_ids_to_be_removed.clear();

    // Cursor starts at the top left
    Vec2 cursor = Vec2(-1, 1);
    for (auto request : c->requests)
    {
        auto global_transform = get_widget_global_transform(c, request.id);

        draw_render_request(request, global_transform);
        destroy_render_request(request);
    }

    for (auto &[_, array] : c->cache_arrays)
    {
        array->add_active_ids(c->widget_ids_to_be_removed);
    }

    c->requests.clear();
    c->widget_layouts.clear();
}

void IMGUI::begin_layout_with_children(Context *c, UI_ID id, Layout &layout, bool keep_old_layout)
{
    widget_accessed(c, id);
    c->index.top()++;
    if (c->parent != NO_ID)
    {
        auto &parent_layout = get_widget_layout(c, c->parent);
        layout_add_child(parent_layout, id);
        layout.parent = c->parent;
    }
    c->parent = id;
    c->index.push(0);
    if (keep_old_layout)
    {
        if (c->widget_layouts.find(id) != c->widget_layouts.end())
            return;
    }
    c->widget_layouts[id] = layout;
}

IMGUI::Layout &IMGUI::end_layout_with_children(Context *c)
{
    auto parent = c->parent;
    assert(parent != NO_ID && "Cannot end layout! There is no parent widget!");
    auto &parent_layout = get_widget_layout(c, parent);
    c->parent = parent_layout.parent;
    c->index.pop();
    return parent_layout;
}

IMGUI::Constraints IMGUI::get_constraints(Context *c, UI_ID id)
{
    auto parent = c->parent;
    if (parent == NO_ID)
    {
        return Constraints(Vec2(0), RenderSystem::get_dimensions(GAME));
    }
    auto parent_layout = get_widget_layout(c, parent);
    return layout_get_constraints(parent_layout, id);
}

void IMGUI::layout_widget(Context *c, UI_ID id, Layout l)
{
    widget_accessed(c, id);
    c->index.top()++;
    if (c->parent != NO_ID)
    {
        auto &parent_layout = get_widget_layout(c, c->parent);
        layout_add_child(parent_layout, id);
        l.parent = c->parent;
    }

    if (c->widget_layouts.find(id) == c->widget_layouts.end())
    {
        c->widget_layouts.insert({id, l});
    }
    else
    {
        c->widget_layouts.at(id) = l;
    }
}

bool IMGUI::widget_layed_out(Context *c, UI_ID id)
{
    return c->widget_layouts.find(id) != c->widget_layouts.end();
}

IMGUI::Layout &IMGUI::get_widget_layout(Context *c, UI_ID id)
{
    assert(widget_layed_out(c, id) && "Layout not found!");
    return c->widget_layouts.at(id);
}

bool IMGUI::mouse_over(Context *c, UI_ID id, Vec2 size)
{
    if (c->widget_transforms.find(id) == c->widget_transforms.end(id))
        return false;

    auto top_left = c->widget_transforms.at(id).position;
    auto mp = InputSystem::get_mouse_position();
    mp.y = 1 - mp.y;
    mp *= RenderSystem::get_dimensions(GAME);
    return mp.x > top_left.x && mp.x < top_left.x + size.x && mp.y > top_left.y && mp.y < top_left.y + size.y;
}

void IMGUI::submit_render_request(Context *c, UI_ID id, const RenderRequest &r)
{
    c->requests.push_back(r);
}

void IMGUI::draw_rect_absolute(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material)
{

    RenderRequest request = {
        .type = RenderRequest::ABSOLUTE_MESH_DRAW,
        .material = material,
    };
    request.data.mesh = c->renderer.quad;
    request.local_transform = {.position = position, .scale = size};
    request.id = id;
    request.z_index = c->z_index++;
    submit_render_request(c, id, request);
}

IMGUI::Transform IMGUI::get_widget_global_transform(Context *c, UI_ID id)
{
    if (c->widget_transforms.find(id) != c->widget_transforms.end())
        return c->widget_transforms[id];
    auto &layout = get_widget_layout(c, id);
    if (layout.parent == NO_ID)
    {
        c->widget_transforms.insert({id, Transform()});
        return Transform();
    }
    auto &parent_layout = get_widget_layout(c, layout.parent);
    auto local_transform = get_child_transform(parent_layout, id);
    auto global_transform = local_transform;
    if (parent_layout.parent != NO_ID)
    {
        global_transform = local_transform + get_widget_global_transform(c, layout.parent);
    }
    c->widget_transforms.insert({id, global_transform});
    return global_transform;
}

void IMGUI::draw_rect(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material, bool clip)
{
    RenderRequest request = {
        .type = RenderRequest::MESH_DRAW,
        .clip = clip,
        .material = material,
    };
    request.data.mesh = c->renderer.quad;
    request.local_transform = {.position = position, .scale = size};
    request.z_index = c->z_index++;
    request.id = id;
    submit_render_request(c, id, request);
}

void IMGUI::draw_batch(Context *c, UI_ID id, QuadBatch *batch, u32 quads, Material *material)
{
    auto layout = get_widget_layout(c, id);
    auto size = gl_get_size(layout.local_size);
    auto pos = Vec2(1, -1) - size * Vec2(1, -1);

    RenderRequest request = {
        .type = RenderRequest::BATCH_DRAW,
        .material = material,
    };
    request.data.batch = batch;
    request.data.num_quads = quads;
    request.local_transform = {.position = pos, .scale = size};
    request.id = id;
    request.z_index = c->z_index++;
    submit_render_request(c, id, request);
}
