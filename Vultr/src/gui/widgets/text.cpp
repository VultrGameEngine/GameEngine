#include <gui/widgets/text.h>
#include <helpers/font_importer.h>
#include <core/system_providers/render_system_provider.h>
#include <gui/materials/default_gui_material.h>
#include <gui/materials/default_batch_material.h>

using namespace Vultr;

#define __text_cache_id ui_id(__FILE__)

template <>
void IMGUI::destroy_cache<IMGUI::TextState>(TextState &cache)
{
    destroy_quad_batch(cache.batch);
}

static void layout_text(IMGUI::Context *c, IMGUI::Layout &l)
{
    using namespace IMGUI;
    auto &state = get_widget_cache<TextState>(c, __text_cache_id, l.owner);
}

static f32 get_text_top_padding(Font *font, const IMGUI::TextStyle &style)
{
    return font->GetHeight(style.font_size) * 0.3;
}

static f32 get_height(Font *f, IMGUI::TextStyle style)
{
    f32 font_height = f->GetHeight(style.font_size);

    f32 top_padding = font_height * 0.3;

    return font_height * 0.5 * style.line_spacing + top_padding;
}

f32 IMGUI::text_get_height(Context *c, const TextStyle &style)
{
    return get_height(c->font, style) + get_text_top_padding(c->font, style);
}

struct Word
{
    std::string text;
    f32 width;
};

static Word text_get_word(Font *font, const IMGUI::TextStyle &style, const std::string &word)
{
    f32 size = 0;
    for (s32 i = 0; i < word.size(); i++)
    {
        auto character = font->GetCharacter(word[i], style.font_size);
        size += character.advance.x;
    }

    return {
        .text = word,
        .width = size,
    };
}

static std::vector<Word> split_text(Font *font, const IMGUI::TextStyle &style, const std::string &text)
{
    std::vector<Word> words;

    // Split up our string into words
    std::string current_word = "";
    u32 index = 0;
    for (char c : text)
    {
        current_word += c;
        if (c == ' ' || index == text.size() - 1)
        {
            auto word = text_get_word(font, style, current_word.c_str());
            words.push_back(word);
            current_word = "";
        }
        index++;
    }

    return words;
}

typedef std::vector<Word> TextLine;

typedef std::vector<TextLine> TextParagraph;

static TextParagraph get_lines_of_text(const TextLine &line, f32 max_width, s32 max_lines = -1)
{
    TextParagraph paragraph;

    f32 total_width = 0;

    TextLine current_line;

    u32 index = 0;
    for (auto &word : line)
    {
        bool last_element = index == line.size() - 1;
        if (total_width + word.width > max_width)
        {
            if (max_lines <= 0 || paragraph.size() < max_lines)
            {
                if (current_line.size() != 0)
                {
                    paragraph.push_back(current_line);

                    current_line = {};

                    total_width = 0;
                }
            }
        }

        current_line.push_back(word);
        total_width += word.width;

        if (last_element)
        {
            paragraph.push_back(current_line);
        }
        index++;
    }

    return paragraph;
}

static std::string get_text_line_text(const TextLine &line)
{
    std::string text = "";
    for (auto &word : line)
    {
        text += word.text;
    }
    return text;
}

static u32 get_paragraph_character_count(const TextParagraph &paragraph)
{
    u32 count = 0;
    for (auto &line : paragraph)
    {
        for (auto &word : line)
        {
            count += word.text.size();
        }
    }
    return count;
}

static void push_text_vertices(IMGUI::Context *c, IMGUI::TextState &state, const TextParagraph &paragraph, Font *font, const IMGUI::TextStyle &style)
{
    using namespace IMGUI;
    auto texture_dimensions = font->texture_dimensions;

    auto character_count = get_paragraph_character_count(paragraph);

    auto *quads = static_cast<Quad *>(malloc(sizeof(Quad) * character_count));

    auto cursor = Vec2(0, get_height(font, style));

    auto text_height = text_get_height(c, style);

    u32 quad_index = 0;
    for (auto &line : paragraph)
    {
        auto text = get_text_line_text(line);
        for (char c : text)
        {
            auto &quad = quads[quad_index];
            quad = Quad();
            auto character = font->GetCharacter(c, style.font_size);
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
            quad_index++;
        }
        cursor.y += text_height;
        cursor.x = 0;
    }

    state.size = Vec2(cursor.x, text_height);

    quad_batch_push_quads(state.batch, quads, character_count);

    free(quads);
}

void IMGUI::text(Context *c, UI_ID id, const std::string &text, TextStyle style)
{
    auto &state = get_widget_cache<TextState>(c, __text_cache_id, id);
    if (state.batch == nullptr)
    {
        state.batch = new_quad_batch();
    }

    auto *font = c->font;

    // If the text is exactly the same, then nothing needs to be done
    if (state.text != text || state.text == "")
    {
        auto words = split_text(c->font, style, text);

        auto constraints = get_constraints(c, id);

        auto lines = get_lines_of_text(words, constraints.max_width);
        push_text_vertices(c, state, lines, font, style);
    }

    auto layout = new_no_child_layout(id, state.size);
    layout_widget(c, id, layout);

    if (style.highlight_color.value.a > 0)
    {
        IMGUI::draw_rect(c, id, Vec2(0), state.size, new_gui_material(c, style.highlight_color));
    }

    auto *mat = new_batch_material(c, font->texture);

    draw_batch(c, id, state.batch, text.size(), mat);
}
