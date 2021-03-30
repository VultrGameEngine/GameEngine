#include <helpers/font_importer.h>
#include <helpers/path.h>
#include <algorithm>
#include <glad/glad.h>
#include <rendering/models/texture.h>

namespace Vultr
{
Font *FontImporter::ImportFont(const std::string &path, const FT_Library &library,
                               double height)
{
    int w = 0;
    int h = 0;
    Font *font = new Font();
    FT_Face face;
    if (FT_New_Face(library, Path::GetFullPath(path).c_str(), 0, &face))
    {
        assert("Failed to load font");
        return font;
    }

    FT_Set_Pixel_Sizes(face, 0, height);

    FT_GlyphSlot g = face->glyph;

    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Find the minimum size for the texture atlas
    for (unsigned char c = 32; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Loading character %c failed!\n", c);
            continue;
        }

        w += g->bitmap.width;
        if (g->bitmap.rows > h)
        {
            h = g->bitmap.rows;
        }
    }

    font->height = (double)face->size->metrics.height / 64 / 2000;
    font->texture_dimensions = glm::vec2(w, h);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x = 0;

    // Load in all of the characters
    for (unsigned char c = 32; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Loading character %c failed!\n", c);
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        font->characters[c] = FontCharacter();

        font->characters[c].advance.x = g->advance.x >> 6;
        font->characters[c].advance.y = g->advance.y >> 6;

        font->characters[c].size.x = g->bitmap.width;
        font->characters[c].size.y = g->bitmap.rows;

        font->characters[c].bearing.x = g->bitmap_left;
        font->characters[c].bearing.y = g->bitmap_top;

        font->characters[c].advance /= glm::vec2(2000, 2000);
        font->characters[c].size /= glm::vec2(2000, 2000);
        font->characters[c].bearing /= glm::vec2(2000, 2000);

        font->characters[c].uv = (float)x / w;

        if (c == 'A')
        {
        }

        x += g->bitmap.width;
    }

    Texture *texture = new Texture(GL_TEXTURE_2D, tex);
    font->texture = texture;

    return font;
}

} // namespace Vultr
