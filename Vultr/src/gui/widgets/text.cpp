#include <gui/widgets/text.h>
#include <helpers/font_importer.h>
#include <core/system_providers/render_system_provider.h>

using namespace Vultr;

#define __text_cache_id ui_id(__FILE__)

void IMGUI::text(Context *c, UI_ID id, std::string text, Vec2 position, TextParams params)
{
    auto &state = get_widget_cache<TextState>(c, __text_cache_id, id);
    if (state.batch == nullptr)
    {
        state.batch = new_quad_batch();
    }

    // If the text is exactly the same, then nothing needs to be done
    // if (state.text == text)
    // {
    //     draw_batch(c, state.batch, text.size());
    //     state.text = text;
    //     return;
    // }

    auto *font = c->font;
    auto texture_dimensions = font->texture_dimensions;

    auto *quads = static_cast<Quad *>(malloc(sizeof(Quad) * text.size()));

    auto cursor = Vec2(0, font->GetHeight(params.font_size) * 0.75 * params.line_spacing);
    for (s32 i = 0; i < text.size(); i++)
    {
        auto &quad = quads[i];
        quad = Quad();
        auto character = font->GetCharacter(text[i], params.font_size);
        auto uv = Vec2(character.uv, 0);
        auto uv_dimensions = character.size * Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR) / Vec2(params.font_size);

        double left = uv.x;
        double right = uv.x + uv_dimensions.x / texture_dimensions.x;

        double top = uv.y;
        double bottom = uv.y + uv_dimensions.y / texture_dimensions.y;

        auto character_position = position + cursor + Vec2(character.bearing.x, -character.bearing.y);

        set_quad_transform(quad, character_position, character.size);

        quad.vertices[0].uv = Vec2(left, bottom);
        quad.vertices[1].uv = Vec2(left, top);
        quad.vertices[2].uv = Vec2(right, bottom);
        quad.vertices[3].uv = Vec2(right, top);

        set_quad_color(quad, params.color);
        set_quad_texture_slot(quad, 0);

        cursor.x += character.advance.x;
    }

    if (params.highlight_color.value.a > 0)
    {
        IMGUI::draw_rect(c, params.highlight_color, position, Vec2(cursor.x, c->font->GetHeight(12) * params.line_spacing));
    }

    quad_batch_push_quads(state.batch, quads, text.size());

    font->texture->Bind(GL_TEXTURE0);

    s32 samplers[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    c->renderer.batch_gui_shader->Bind();
    glUniform1iv(c->renderer.batch_gui_shader->GetUniformLocation("u_Textures"), 16, samplers);

    draw_batch(c, state.batch, text.size());
}
