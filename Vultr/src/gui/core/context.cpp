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

static void submit_stencil_request(IMGUI::Context *c, IMGUI::UI_ID id, const IMGUI::StencilRequest &request)
{
    // Stencils have to be attached to layouts with children, otherwise the gui system will have no idea what children to apply a stencil to
    assert(request.id == c->parent && "Stencils have to be attached to layouts with children!");

    // Add the request to our array
    c->stencil_requests[++c->stencil_request_index] = request;

    // Map it properly
    c->layout_to_stencil[id] = c->stencil_request_index;
    c->current_stencil = &c->stencil_requests[c->stencil_request_index];
}

void IMGUI::begin(Context *c, const UpdateTick &t)
{
    auto dimensions = RenderSystem::get_dimensions(GAME);
    glViewport(0, 0, dimensions.x, dimensions.y);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glDisable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SCISSOR_TEST);
    // glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // All widgets will pass the stencil test by default
    glStencilMask(0x00);               // Don't update stencil buffer by default

    update_mouse_state(c->left_mb, Input::MOUSE_LEFT);
    update_mouse_state(c->right_mb, Input::MOUSE_RIGHT);
    update_mouse_state(c->middle_mb, Input::MOUSE_MIDDLE);

    c->delta_time = t.m_delta_time;
    c->z_index[0] = 0;
    c->widget_depth = 0;
    c->drawing_id = NO_ID;
    c->index = std::stack<s32>();
    c->index.push(0);
    c->layout_to_stencil.clear();
    c->current_stencil = nullptr;
    c->stencil_request_index = 0;
}

void IMGUI::widget_accessed(Context *c, UI_ID id)
{
    c->widget_ids_to_be_removed.erase(id);
}

static void get_gl_transform(const IMGUI::Transform &local_transform, const IMGUI::Transform &global_transform, Vec3 &gl_global_pos, Vec3 &gl_global_size, Mat4 &full_transform)
{
    using namespace IMGUI;
    Vec3 local_scale = Vec3(gl_get_size(local_transform.scale), 1);
    Vec3 global_position = Vec3(gl_get_position(global_transform.position, local_scale), 0);
    Vec3 local_position = Vec3(gl_get_raw_position(local_transform.position), 0);
    Vec2 global_scale = glm::scale(Vec3(global_transform.scale, 1.0)) * Vec4(local_scale.x, local_scale.y, 0.0, 1.0);
    gl_global_pos = global_position + local_position;
    gl_global_size = Vec3(global_scale, 1);
    full_transform = glm::translate(gl_global_pos) * glm::scale(gl_global_size);
}

static u8 apply_stencil(IMGUI::Context *c, IMGUI::StencilRequest *request, u8 index = 0, bool is_recurse = false)
{
    using namespace IMGUI;
    if (request != nullptr)
    {
        u8 val = apply_stencil(c, request->parent, index + 1, true);

        // When drawing a stencil there shouldn't be any color
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // Bind the material
        request->data.material->bind();
        Mat4 full_transform;
        Vec3 gl_global_pos;
        Vec3 gl_global_size;
        Transform global_transform = get_widget_global_transform(c, request->id);
        get_gl_transform(request->local_transform, global_transform, gl_global_pos, gl_global_size, full_transform);

        set_uniform_matrix_4fv(request->data.material->shader, "transform", glm::value_ptr(full_transform));
        request->data.mesh->Draw();

        if (!is_recurse)
        {
            // When we finish drawing a stencil, we need to update our stencil function so that all subsequent draw calls will only draw when the stencil buffer is 1
            glStencilFunc(GL_EQUAL, val, 0xFF);
        }

        // Also re-enable color writing
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        return val;
    }
    else
    {
        // We want to clear the stencil buffer when there is no stencil request, as this is our starting stencil
        glClear(GL_STENCIL_BUFFER_BIT);

        // When there is no stencil, then never discard
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        return index;
    }
}

// static void apply_stencil(IMGUI::Context *c, IMGUI::StencilRequest *request, IMGUI::RenderRequestContext *context)
// {
//     using namespace IMGUI;
// }

// static size_t get_stencil_parent_count(IMGUI::StencilRequest *request)
// {
//     if (request == nullptr)
//     {
//         return 0;
//     }

//     size_t count = 0;
//     auto *r = request;
//     while (request->parent != nullptr)
//     {
//         count++;
//         r = request->parent;
//     }
//     return count;
// }

// static size_t find_stencil_parent(IMGUI::StencilRequest *request, IMGUI::StencilRequest *parent)
// {
//     if (request == nullptr)
//     {
//         return 0;
//     }
//     size_t count = 0;
//     auto *r = request;
//     while (r != parent && r->parent != nullptr)
//     {
//         r = r->parent;
//     }
//     return count;
// }

void draw_render_request(IMGUI::Context *c, IMGUI::RenderRequestContext *context, const IMGUI::RenderRequest &r, IMGUI::Transform global_transform)
{
    using namespace IMGUI;
    Mat4 full_transform;
    Vec3 gl_global_pos;
    Vec3 gl_global_size;
    if (r.type == RenderRequest::ABSOLUTE_MESH_DRAW)
    {
        // full_transform = get_transform_matrix(r.transform);
    }
    else if (r.type == RenderRequest::MESH_DRAW)
    {
        get_gl_transform(r.local_transform, global_transform, gl_global_pos, gl_global_size, full_transform);
    }
    else if (r.type == RenderRequest::BATCH_DRAW)
    {
        Vec3 local_scale = Vec3(r.local_transform.scale, 1);
        Vec3 global_position = Vec3(gl_get_position(global_transform.position, local_scale), 0);
        Vec3 local_position = Vec3(r.local_transform.position, 0);
        Vec2 inverted_global_scale = Vec2(1) - global_transform.scale;
        Vec3 offset = Vec3(inverted_global_scale * (Vec2(1) - Vec2(local_scale)) * Vec2(1, -1), 0);
        gl_global_pos = global_position + local_position - offset;
        gl_global_size = Vec3(global_transform.scale, 1.0);
        full_transform = glm::translate(gl_global_pos) * glm::scale(gl_global_size);
    }
    else
    {
        throw("???");
        return;
    }

    if (r.stencil != context->stencil)
    {
        glStencilMask(0xFF); // We need to enable writing to the stencil buffer whenever drawing a stencil
        apply_stencil(c, r.stencil);
        glStencilMask(0x00); // And then disable writin after we are done

        context->stencil = r.stencil;
    }

    r.material->bind();
    set_uniform_matrix_4fv(r.material->shader, "transform", glm::value_ptr(full_transform));

    // if (r.clip)
    // {
    //     auto screen_global_size = screen_get_size_from_gl(gl_global_size);
    //     auto screen_global_pos = screen_get_position_from_gl(gl_global_pos, gl_global_size);
    //     glScissor(screen_global_pos.x, screen_global_pos.y, screen_global_size.x, screen_global_size.y);
    // }

    if (r.type == RenderRequest::ABSOLUTE_MESH_DRAW || r.type == RenderRequest::MESH_DRAW)
    {
        r.data.mesh->Draw();
    }
    else
    {
        quad_batch_draw(r.data.batch, r.data.num_quads);
    }
    // if (r.clip)
    // {
    //     auto dimensions = RenderSystem::get_dimensions(GAME);
    //     glScissor(0, 0, dimensions.x, dimensions.y);
    // }
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

    RenderRequestContext rc = {};

    // Cursor starts at the top left
    Vec2 cursor = Vec2(-1, 1);
    for (auto request : c->requests)
    {
        auto global_transform = get_widget_global_transform(c, request.id);

        draw_render_request(c, &rc, request, global_transform);
        destroy_render_request(request);
    }

    for (auto &[_, array] : c->cache_arrays)
    {
        array->add_active_ids(c->widget_ids_to_be_removed);
    }

    c->requests.clear();
    c->widget_layouts.clear();
}

void IMGUI::begin_layout_with_children(Context *c, UI_ID id, Layout &layout)
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
    c->widget_layouts[id] = layout;
    branch_z_index(c);
}

IMGUI::Layout &IMGUI::end_layout_with_children(Context *c, Widget_ID widget)
{
    auto parent = c->parent;
    assert(parent != NO_ID && "Cannot end layout! There is no parent widget!");
    auto &parent_layout = get_widget_layout(c, parent);

    assert(parent_layout.widget_type == widget && "Failed to end layout! Widget type does not match! Are you missing an `end` widget somewhere?");

    // If the current stencil request matches the widget that we are currently finishing up laying out
    if (c->current_stencil != nullptr && c->current_stencil->id == parent)
    {
        // Then move up one stencil request
        c->current_stencil = c->current_stencil->parent;
    }

    c->parent = parent_layout.parent;
    c->index.pop();
    exit_branch(c);

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

s32 IMGUI::get_z_index(Context *c)
{
    return c->z_index[c->widget_depth];
}

s32 IMGUI::increase_z(Context *c)
{
    return ++c->z_index[c->widget_depth];
}

s32 IMGUI::decrease_z(Context *c)
{
    return --c->z_index[c->widget_depth];
}

s32 IMGUI::branch_z_index(Context *c)
{
    assert(c->widget_depth < MAX_WIDGET_DEPTH && "HOW DO YOU HAVE MORE THAN 1000 CHILDREN????");
    // Increase the widget depth
    c->widget_depth++;

    // Set the new z_index to be one increased from the previous z_index
    c->z_index[c->widget_depth] = c->z_index[c->widget_depth - 1] + 1;
    return c->z_index[c->widget_depth];
}

void IMGUI::exit_branch(Context *c)
{
    c->widget_depth--;
}

void IMGUI::layout_widget(Context *c, UI_ID id, Layout l)
{
    assert(l.type == Layout::NO_CHILD && "To layout a widget with more than one child, use `begin_layout_with_children` and `end_layout_with_children`");
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
        .stencil = c->current_stencil,
    };
    request.data.mesh = c->renderer.quad;
    request.local_transform = {.position = position, .scale = size};
    request.id = id;
    request.z_index = c->z_index[c->widget_depth]++;
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
        .stencil = c->current_stencil,
    };
    request.data.mesh = c->renderer.quad;
    request.local_transform = {.position = position, .scale = size};
    request.z_index = increase_z(c);
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
        .stencil = c->current_stencil,
    };
    request.data.batch = batch;
    request.data.num_quads = quads;
    request.local_transform = {.position = pos, .scale = size};
    request.id = id;
    request.z_index = increase_z(c);
    submit_render_request(c, id, request);
}

void IMGUI::begin_stencil(Context *c, UI_ID id)
{
    StencilRequest request = {
        .id = id,
        .parent = c->current_stencil,
    };
    request.data.mesh = c->renderer.quad, submit_stencil_request(c, id, request);
}

void IMGUI::end_stencil(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material)
{
    Transform local_transform = {.position = position, .scale = size};
    auto &stencil = c->stencil_requests[c->layout_to_stencil[id]];
    stencil.data.material = material;
    stencil.local_transform = local_transform;
}
