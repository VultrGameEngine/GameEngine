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
    glm::vec2 size;
    glm::vec2 bearing;
    glm::vec2 advance;
    float uv;
};

class Font
{
  public:
    FontCharacter characters[128];
    double height = 0;
    Texture *texture;

    glm::vec2 texture_dimensions = glm::vec2(0, 0);
    Font()
    {
    }
    ~Font()
    {
        delete texture;
    }
};
} // namespace Vultr
