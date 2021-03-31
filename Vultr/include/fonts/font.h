#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <ft2build.h>
#include <rendering/models/texture.h>
#include FT_FREETYPE_H

namespace Vultr
{
struct FontCharacter
{
    FontCharacter()
        : size(glm::vec2(0, 0)), bearing(glm::vec2(0, 0)), advance(glm::vec2(0, 0))
    {
    }

    FontCharacter(const FontCharacter &other, double scale_factor)
    {
        this->size = other.size * glm::vec2(scale_factor);
        this->bearing = other.bearing * glm::vec2(scale_factor);
        this->advance = other.advance * glm::vec2(scale_factor);
        this->uv = other.uv;
    }
    glm::vec2 size;
    glm::vec2 bearing;
    glm::vec2 advance;
    float uv;
};

class Font
{
  public:
    Texture *texture;

    glm::vec2 texture_dimensions = glm::vec2(0, 0);
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
        delete texture;
    }

  private:
    FontCharacter characters[128];
    double height = 0;
};
} // namespace Vultr
