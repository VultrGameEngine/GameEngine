#include <gui/widgets/text_input.h>
#include <gui/materials/default_gui_material.h>

using namespace Vultr;

#define __text_input_cache_id ui_id(__FILE__)

template <>
void IMGUI::initialize_cache(IMGUI::TextInputState &cache)
{
    InputSystem::set_key_listener([&cache](KeyEvent event) {
        if (event.action == Input::PRESS || event.action == Input::REPEAT)
        {
            TextInputEvent::Type type;
            if (event.key == Input::KEY_BACKSPACE)
            {
                type = TextInputEvent::BACKSPACE;
            }
            else if (event.key == Input::KEY_DELETE)
            {
                type = TextInputEvent::DELETE;
            }
            else if (event.key == Input::KEY_ENTER)
            {
                type = TextInputEvent::ENTER;
            }
            else if (event.key == Input::KEY_LEFT)
            {
                type = TextInputEvent::MOVE_LEFT;
            }
            else if (event.key == Input::KEY_RIGHT)
            {
                type = TextInputEvent::MOVE_RIGHT;
            }

            cache.queued_events.push({
                .type = type,
            });
        }
    });

    InputSystem::set_character_listener([&cache](u32 data) {
        cache.queued_events.push({
            .type = TextInputEvent::KEY,
            .data = char(data),
        });
    });
}

template <>
void IMGUI::destroy_cache<IMGUI::TextInputState>(TextInputState &cache){};

void IMGUI::text_input(Context *c, UI_ID id, std::string &text, TextInputStyle style)
{
    // Get our cache
    auto &state = get_widget_cache<TextInputState>(c, __text_input_cache_id, id);

    // If we had some key events then we need to use them here
    while (state.queued_events.size() > 0)
    {
        auto event = state.queued_events.front();
        switch (event.type)
        {
                // If there was a key event, add whatever character there was
            case TextInputEvent::KEY:
                text += event.data;
                break;
            case TextInputEvent::BACKSPACE:
                text.erase(text.size() - 1);
                break;
            case TextInputEvent::DELETE:
                break;
            case TextInputEvent::ENTER:
                break;
            case TextInputEvent::MOVE_LEFT:
                break;
            case TextInputEvent::MOVE_RIGHT:
                break;
        }
        // Remove the event so we keep going
        state.queued_events.pop();
    }

    // Get the parent constraints
    auto parent_constraints = get_constraints(c, id);

    // Get the max size for our widget. This will be the width of our text input
    auto size = constraints_max(parent_constraints);

    auto mcl = new_multi_child_layout(__text_input_cache_id, id, size, parent_constraints, Vec2(0));

    begin_layout_with_children(c, id, mcl);

    auto text_parent_id = id + 19309 + __LINE__;
    auto text_id = id + 19309 + __LINE__;
    IMGUI::begin_padding(c, text_parent_id, style.padding_style);
    {
        IMGUI::text(c, text_id, text, style.text_style);
    }
    IMGUI::end_padding(c);

    auto &layout = end_layout_with_children(c, __text_input_cache_id);
    auto &data = get_layout_data<MultiChildLayout>(layout);

    data.children_transforms[text_parent_id] = {.position = Vec2(0)};
    size.y = get_widget_layout(c, text_parent_id).local_size.y;
    layout.local_size = size;

    auto text_width = get_widget_layout(c, text_id).local_size.x;
    const auto &insets = style.padding_style.insets;

    auto *background_mat = new_gui_material(c, style.background_color);
    draw_rect(c, id, Vec2(0), size, background_mat, true);

    auto *cursor_mat = new_gui_material(c, style.cursor_color);
    draw_rect(c, id, Vec2(insets.left + text_width, insets.top), Vec2(5, size.y - insets.top - insets.bottom), cursor_mat);
}
