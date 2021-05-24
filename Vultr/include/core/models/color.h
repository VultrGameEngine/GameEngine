#pragma once

struct Color
{
    Color() : value(glm::vec4(0, 0, 0, 1))
    {
    }
    Color(glm::vec4 p_value) : value(p_value)
    {
    }
    glm::vec4 value;
};
