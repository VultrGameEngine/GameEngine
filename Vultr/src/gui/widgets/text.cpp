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

struct TextLine
{
    std::vector<Word> words;
    f32 width = 0;
};

struct TextParagraph
{
    std::vector<TextLine> lines;
    f32 max_width = 0;
};

static TextLine split_text(Font *font, const IMGUI::TextStyle &style, const std::string &text)
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

    return {.words = words};
}

// Add a word to a text line
static void add_word(TextLine &line, const Word &word)
{
    line.words.push_back(word);
    line.width += word.width;
}

// Add a line of text to a paragraph
static void add_line(TextParagraph &paragraph, const TextLine &line)
{
    paragraph.lines.push_back(line);
    if (line.width > paragraph.max_width)
        paragraph.max_width = line.width;
}

static TextParagraph get_lines_of_text(const TextLine &line, f32 max_width, s32 max_lines = 0)
{
    TextParagraph paragraph;
    TextLine current_line;

    // Loop through all of the words in the line
    size_t index = 0;
    for (auto &word : line.words)
    {
        // If we go over the max width...
        if (current_line.width + word.width > max_width)
        {
            // And the number of lines is less than the maximum specified... (if the max lines is 0 or less that means there is no max)
            if (max_lines <= 0 || paragraph.lines.size() < max_lines)
            {
                // And the number of words in the line is not empty
                if (current_line.words.size() != 0)
                {
                    // Then add the line to the paragraph
                    add_line(paragraph, current_line);
                    current_line = {};
                }
            }
        }

        // Add the word to the current line
        add_word(current_line, word);

        // If we have reached the end of our words
        if (index == line.words.size() - 1)
        {
            // Then we want to add whatever remaining line we have to our paragraph. This will happen when the last line doesn't go over the max width
            add_line(paragraph, current_line);
        }

        index++;
    }

    return paragraph;
}

static std::string get_text_line_text(const TextLine &line)
{
    std::string text = "";
    for (auto &word : line.words)
    {
        text += word.text;
    }
    return text;
}

static u32 get_paragraph_character_count(const TextParagraph &paragraph)
{
    u32 count = 0;
    for (auto &line : paragraph.lines)
    {
        for (auto &word : line.words)
        {
            count += word.text.size();
        }
    }
    return count;
}

static void push_text_vertices(IMGUI::Context *c, IMGUI::TextState &state, const TextParagraph &paragraph, Font *font, const IMGUI::TextStyle &style)
{
    using namespace IMGUI;

    // Get the dimensions of the font texture
    auto texture_dimensions = font->texture_dimensions;

    // Get the number of characters so that we can create our buffer of quads
    auto character_count = get_paragraph_character_count(paragraph);

    // Create the buffer of quads
    auto *quads = static_cast<Quad *>(malloc(sizeof(Quad) * character_count));

    // Start our cursor
    auto cursor = Vec2(0, get_height(font, style));

    // Get the height of our each line of text
    auto text_height = text_get_height(c, style);

    // The current quad index which will tick up for each character
    size_t quad_index = 0;

    // Loop through all of the lines in our paragraph
    for (auto &line : paragraph.lines)
    {
        // Get the text line, which we will then create the quads for
        auto text = get_text_line_text(line);

        for (char c : text)
        {
            // Create a new quad in our buffer
            auto &quad = quads[quad_index];
            quad = Quad();

            // We get the character using our font size
            auto character = font->GetCharacter(c, style.font_size);

            // Get the UV top left posiiton
            auto uv = Vec2(character.uv, 0);

            // And the UV size
            auto uv_dimensions = character.size * Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR) / Vec2(style.font_size);

            // Get the UV positions
            f32 left = uv.x;
            f32 right = uv.x + uv_dimensions.x / texture_dimensions.x;
            f32 top = uv.y;
            f32 bottom = uv.y + uv_dimensions.y / texture_dimensions.y;

            // Add the character bearings to the cursor position to get our character position
            auto character_position = cursor + Vec2(character.bearing.x, -character.bearing.y);

            // Then set the quad position and size
            set_quad_transform(quad, character_position, character.size);

            // Update the UVs
            quad.vertices[0].uv = Vec2(left, bottom);
            quad.vertices[1].uv = Vec2(left, top);
            quad.vertices[2].uv = Vec2(right, bottom);
            quad.vertices[3].uv = Vec2(right, top);

            // Set color nd texture
            set_quad_color(quad, style.font_color);
            set_quad_texture_slot(quad, 0);

            // Move the cursor
            cursor.x += character.advance.x;

            // Go on to the next quad
            quad_index++;
        }

        // After each line, we want to move the cursor down by the line height and reset the cursor to the beginning of the line
        cursor.y += text_height;
        cursor.x = 0;
    }

    // Set the size of our widget
    state.size = Vec2(cursor.x, text_height * paragraph.lines.size());

    // Push the quads
    quad_batch_push_quads(state.batch, quads, character_count);

    // And free our malloced array
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
        // Get the list of words and their widths
        auto words = split_text(c->font, style, text);

        // Get the constraints
        auto constraints = get_constraints(c, id);

        // The format our words based on the max width. This will split our words into lines
        auto lines = get_lines_of_text(words, constraints.max_width);

        // Then use the lines to create the vertices
        push_text_vertices(c, state, lines, font, style);
    }

    // Create the layout
    auto layout = new_no_child_layout(id, state.size);
    layout_widget(c, id, layout);

    if (style.highlight_color.value.a > 0)
    {
        IMGUI::draw_rect(c, id, Vec2(0), state.size, new_gui_material(c, style.highlight_color));
    }

    auto *mat = new_batch_material(c, font->texture);

    draw_batch(c, id, state.batch, text.size(), mat);
}
