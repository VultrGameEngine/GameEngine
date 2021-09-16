#include <helpers/font_importer.h>
#include <algorithm>
#include <glad/glad.h>
#include <rendering/types/texture.h>

namespace Vultr
{
    Font *FontImporter::import_font(const FontSource &source, const FT_Library &library)
    {
        int w = 0;
        int h = 0;
        Font *font = new Font();
        FT_Face face;
        if (FT_New_Face(library, source.path, 0, &face))
        {
            assert("Failed to load font");
            return font;
        }

        FT_Set_Pixel_Sizes(face, 0, 100);

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

        font->SetHeight((double)face->size->metrics.height / 64 / TEXT_SCALE_FACTOR);
        font->texture_dimensions = Vec2(w, h);
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

            glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            FontCharacter character = FontCharacter();

            character.advance.x = g->advance.x >> 6;
            character.advance.y = g->advance.y >> 6;

            character.size.x = g->bitmap.width;
            character.size.y = g->bitmap.rows;

            character.bearing.x = g->bitmap_left;
            character.bearing.y = g->bitmap_top;

            character.advance /= Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR);
            character.size /= Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR);
            character.bearing /= Vec2(TEXT_SCALE_FACTOR, TEXT_SCALE_FACTOR);

            character.uv = (float)x / w;

            font->SetCharacter(c, character);

            x += g->bitmap.width;
        }

        // TODO: Reimplement
        // Texture texture = {
        //     .id = tex,
        //     .type = GL_TEXTURE_2D,
        // };
        // font->texture = texture;
        FT_Done_Face(face);

        return font;
    }

} // namespace Vultr
