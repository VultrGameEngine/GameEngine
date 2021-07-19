#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <ft2build.h>
#include <rendering/types/texture.h>
#include FT_FREETYPE_H

namespace Vultr
{
    struct FontCharacter
    {
        FontCharacter() : size(Vec2(0, 0)), bearing(Vec2(0, 0)), advance(Vec2(0, 0)), uv(0)
        {
        }

        FontCharacter(const FontCharacter &other, double scale_factor)
        {
            this->size = other.size * Vec2(scale_factor);
            this->bearing = other.bearing * Vec2(scale_factor);
            this->advance = other.advance * Vec2(scale_factor);
            this->uv = other.uv;
        }
        Vec2 size;
        Vec2 bearing;
        Vec2 advance;
        float uv;
    };

    class Font
    {
      public:
        Texture texture;

        Vec2 texture_dimensions = Vec2(0, 0);
        Font()
        {
        }
        void SetCharacter(char character, const FontCharacter &c)
        {
            characters[character] = c;
        }

        void SetHeight(double height)
        {
            this->height = height;
        }

        FontCharacter GetCharacter(char c, double scale_factor)
        {
            return FontCharacter(characters[c], scale_factor);
        }

        double GetHeight(double scale_factor)
        {
            return height * scale_factor;
        }

        ~Font()
        {
            delete_texture(texture);
        }

      private:
        FontCharacter characters[128];
        double height = 0;
    };
} // namespace Vultr
