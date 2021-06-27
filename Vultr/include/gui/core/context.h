#pragma once
#include "window.h"
#include <gui/rendering/renderer.h>
#include <gui/rendering/render_request.h>
#include <gui/rendering/quad_batch.h>
#include "ui_id.h"
#include <stack>
#include <input/input.h>
#include "widget_cache.h"
#include <core/models/update_tick.h>
#include <rendering/models/texture.h>
#include <fonts/font.h>
#include <gui/layout/layout.h>
#include <gui/layout/constraints.h>

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

        void update_mouse_state(MouseState &m, Input::MouseButton button);

        struct Context
        {
            Window window;

            Vec2 cursor_position;
            Vec2 cusor_dimension;

            Renderer renderer;

            UI_ID hot = NO_ID;
            UI_ID active = NO_ID;

            MouseState right_mb;
            MouseState left_mb;
            MouseState middle_mb;

            f32 delta_time;

            std::unordered_map<WidgetType, IWidgetCache *> cache_arrays{};

            Font *font;

            s32 z_index = 0;

            UI_ID parent = NO_ID;

            // This is the index that will increase every time that a widget is layed out when there is a parent
            std::stack<s32> index;

            std::unordered_map<UI_ID, Layout> widget_layouts;

            std::unordered_map<UI_ID, Transform> widget_transforms;

            std::vector<RenderRequest> requests;

            UI_ID drawing_id = NO_ID;
        };

        Context *new_context(const Window &window);

        void context_destroy_widget(Context *c, UI_ID id);

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
            c->drawing_id = id;
            auto *cache_array = get_widget_cache_array<T>(c, type);
            if (!cache_array->has_data(id))
            {
                cache_array->insert_data(id);
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

        bool widget_layed_out(Context *c, UI_ID id);
        Layout &get_widget_layout(Context *c, UI_ID id);
        Constraints get_constraints(Context *c, UI_ID id);
        Transform get_widget_global_transform(Context *c, UI_ID id);

        void begin_layout_with_children(Context *c, UI_ID id, Layout &layout, bool keep_old_layout = true);
        Layout &end_layout_with_children(Context *c);

        //
        //
        //
        //
        // UTILS
        bool mouse_over(Context *c, UI_ID id, Vec2 size);

        void submit_render_request(Context *c, UI_ID id, const RenderRequest &r);

        void draw_rect_absolute(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material);

        void draw_rect(Context *c, UI_ID id, Vec2 position, Vec2 size, Material *material);
        void draw_batch(Context *c, UI_ID id, QuadBatch *batch, u32 quads, Material *material);

        bool is_hot(Context *c, UI_ID id);
        bool is_active(Context *c, UI_ID id);

        void set_hot(Context *c, UI_ID id);
        void set_not_hot(Context *c, UI_ID id);
        void set_active(Context *c, UI_ID id);
        void set_not_active(Context *c, UI_ID id);

    } // namespace IMGUI
} // namespace Vultr
