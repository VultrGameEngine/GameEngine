#pragma once
#include "window.h"
#include <gui/rendering/renderer.h>
#include <gui/rendering/render_request.h>
#include "ui_id.h"
#include <stack>
#include <input/input.h>
#include "widget_cache.h"
#include <core/models/update_tick.h>

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

            UI_ID hot;
            UI_ID active;

            MouseState right_mb;
            MouseState left_mb;
            MouseState middle_mb;

            f32 delta_time;

            std::unordered_map<WidgetType, IWidgetCache *> cache_arrays{};
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
            auto *cache_array = get_widget_cache_array<T>(c, type);
            if (!cache_array->has_data(id))
            {
                cache_array->insert_data(id, T{});
            }
            return cache_array->get_data(id);
        }

        void begin(Context *c, const UpdateTick &t);
        void end(Context *c);

        bool mouse_over(Vec2 top_left, Vec2 size);

        void draw_rect(Context *c, Vec4 color, Vec2 position, Vec2 dimensions, Shader *shader = nullptr);

        bool is_hot(Context *c, UI_ID id);
        bool is_active(Context *c, UI_ID id);

        void set_hot(Context *c, UI_ID id);
        void set_not_hot(Context *c, UI_ID id);
        void set_active(Context *c, UI_ID id);
        void set_not_active(Context *c, UI_ID id);

    } // namespace IMGUI
} // namespace Vultr
