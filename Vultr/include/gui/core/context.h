#pragma once
#include "window.h"
#include <gui/rendering/renderer.h>
#include <gui/rendering/render_request.h>
#include <gui/rendering/stencil_request.h>
#include <gui/rendering/quad_batch.h>
#include "ui_id.h"
#include <stack.h>
#include <dynamic_array.h>
#include <input/input.h>
#include "widget_cache.h"
#include <core/models/update_tick.h>
#include <rendering/types/texture.h>
#include <fonts/font.h>
#include <gui/layout/layout.h>
#include <gui/layout/constraints.h>

#define MAX_WIDGET_DEPTH 1000
#define MAX_STENCIL_REQUESTS 100

namespace Vultr
{
    namespace IMGUI
    {
        struct MouseState
        {
            Input::Action state = Input::Action::RELEASE;

            bool mouse_went_up = false;
            bool mouse_went_down = false;
        };

        struct Context
        {
            Engine *engine;
            Window window;

            Renderer renderer;

            UI_ID hot = NO_ID;
            UI_ID active = NO_ID;

            MouseState right_mb;
            MouseState left_mb;
            MouseState middle_mb;

            f32 delta_time;

            std::unordered_map<WidgetType, IWidgetCache *> cache_arrays{};

            Font *font;

            UI_ID parent = NO_ID;

            // This is the index that will increase every time that a widget is layed out when there is a parent
            vtl::Stack<s32> index;

            std::unordered_map<UI_ID, Layout> widget_layouts;

            std::unordered_map<UI_ID, Transform> widget_transforms;

            vtl::DynamicArray<RenderRequest, 40, 2, 1> requests;

            vtl::DynamicArray<UI_ID> widget_ids_to_be_removed;

            // The Z Index stack
            s32 z_index[MAX_WIDGET_DEPTH];
            size_t widget_depth;

            UI_ID drawing_id = NO_ID;

            // Array of stencil requests
            StencilRequest stencil_requests[MAX_STENCIL_REQUESTS];
            // Index into the stencil_requests array
            size_t stencil_request_index = 0;
            // Hashmap from widget id to stencil_request index
            std::unordered_map<UI_ID, size_t> layout_to_stencil;
            // Cached stencil request from the closest parent with a stencil request
            StencilRequest *current_stencil = nullptr;
        };

        Context *new_context(Engine *e, const Window &window);
        void delete_context(Context *c);

        void context_destroy_widget(Context *c, UI_ID id);

        void widget_accessed(Context *c, UI_ID id);

        template <typename T>
        WidgetCache<T> *get_widget_cache_array(Context *c, WidgetType type)
        {
            if (c->cache_arrays.find(type) == c->cache_arrays.end())
            {
                c->cache_arrays.insert({type, new WidgetCache<T>()});
            }

            return static_cast<WidgetCache<T> *>(c->cache_arrays[type]);
        }

        template <typename T>
        T &get_widget_cache(Context *c, WidgetType type, UI_ID id)
        {
            widget_accessed(c, id);
            c->drawing_id = id;
            auto *cache_array = get_widget_cache_array<T>(c, type);
            if (!cache_array->has_data(id))
            {
                cache_array->insert_data(c, id);
            }
            return cache_array->get_data(id);
        }

        void begin(Context *c, const UpdateTick &t);
        void end(Context *c);

        //
        //
        //
        //
        // LAYOUT
        void layout_widget(Context *c, UI_ID id, Layout layout);

        s32 get_z_index(Context *c);
        s32 increase_z(Context *c);
        s32 decrease_z(Context *c);
        s32 branch_z_index(Context *c);
        void exit_branch(Context *c);

        bool widget_layed_out(Context *c, UI_ID id);
        Layout &get_widget_layout(Context *c, UI_ID id);
        Constraints get_constraints(Context *c, UI_ID id);
        Transform get_widget_global_transform(Context *c, UI_ID id);

        void begin_layout_with_children(Context *c, UI_ID id, Layout &layout);
        Layout &end_layout_with_children(Context *c, Widget_ID widget);

        //
        //
        //
        //
        // UTILS
        // NOTE: Materials are always cleaned up for you, so you don't need to worry about them
        bool mouse_over(Context *c, UI_ID id, Vec2 size);

        void submit_render_request(Context *c, UI_ID id, const RenderRequest &r);

        void draw_rect_absolute(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material);

        void draw_rect(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material, bool clip = false);
        void draw_batch(Context *c, UI_ID id, QuadBatch *batch, u32 quads, Material *material);

        //
        //
        //
        //
        // STENCILS
        // NOTE: The reason that styling options for stenciling is done at the end because a common usecase is to change the stencil based on the children size.
        // There is no downside to this method, because you can always just store the styling at the beginning. It makes less intuitive sense but it works better for more usecases.
        void begin_stencil(Context *c, UI_ID id);
        void end_stencil(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material);

        void begin_scissor(Context *c, UI_ID id);
        void end_scissor(Context *c, UI_ID id, Vec2 position, Vec2 size);

        bool is_hot(Context *c, UI_ID id);
        bool is_active(Context *c, UI_ID id);

        void set_hot(Context *c, UI_ID id);
        void set_not_hot(Context *c, UI_ID id);
        void set_active(Context *c, UI_ID id);
        void set_not_active(Context *c, UI_ID id);
    } // namespace IMGUI
} // namespace Vultr
