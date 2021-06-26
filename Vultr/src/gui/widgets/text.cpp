#include <gui/widgets/text.h>
#include <helpers/font_importer.h>
#include <core/system_providers/render_system_provider.h>
#include <gui/materials/default_gui_material.h>
#include <gui/materials/default_batch_material.h>

using namespace Vultr;

#define __text_cache_id ui_id(__FILE__)

static void layout_text(IMGUI::Context *c, IMGUI::Layout &l)
{
    using namespace IMGUI;
    auto &state = get_widget_cache<TextState>(c, __text_cache_id, l.owner);
}

void IMGUI::text(Context *c, UI_ID id, std::string text, TextStyle style)
{
    auto &state = get_widget_cache<TextState>(c, __text_cache_id, id);
    if (state.batch == nullptr)
    {
        state.batch = new_quad_batch();
    }

    // If the text is exactly the same, then nothing needs to be done
    if (state.text != text)
    {
    }

    auto *font = c->font;
    auto texture_dimensions = font->texture_dimensions;

    auto *quads = static_cast<Quad *>(malloc(sizeof(Quad) * text.size()));

    f32 font_height = font->GetHeight(style.font_size);

    f32 top_padding = font_height * 0.3;

    auto cursor = Vec2(0, font_height * 0.5 * style.line_spacing + top_padding);
    for (s32 i = 0; i < text.size(); i++)
    {
        auto &quad = quads[i];
        quad = Quad();
        auto character = font->GetCharacter(text[i], style.font_size);
        auto uv = Vec2(character.uv, 0);
        auto uv_dimensions = character.size * Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR) / Vec2(style.font_size);

        double left = uv.x;
        double right = uv.x + uv_dimensions.x / texture_dimensions.x;

        double top = uv.y;
        double bottom = uv.y + uv_dimensions.y / texture_dimensions.y;

        auto character_position = cursor + Vec2(character.bearing.x, -character.bearing.y);

        set_quad_transform(quad, character_position, character.size);

        quad.vertices[0].uv = Vec2(left, bottom);
        quad.vertices[1].uv = Vec2(left, top);
        quad.vertices[2].uv = Vec2(right, bottom);
        quad.vertices[3].uv = Vec2(right, top);

        set_quad_color(quad, style.font_color);
        set_quad_texture_slot(quad, 0);

        cursor.x += character.advance.x;
    }

    auto size = cursor + Vec2(0, top_padding);
    auto layout = new_no_child_layout(id, size);
    layout_widget(c, id, layout);

    if (style.highlight_color.value.a > 0)
    {
        IMGUI::draw_rect(c, id, Vec2(0), size, new_gui_material(c, style.highlight_color));
    }

    quad_batch_push_quads(state.batch, quads, text.size());
    auto *mat = new_batch_material(c, font->texture);
    mat->bind();

    draw_batch(c, id, state.batch, text.size(), mat);
    free(quads);
}
