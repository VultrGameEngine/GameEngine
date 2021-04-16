#pragma once
#include <glm/glm.hpp>

namespace Vultr
{
namespace GUI
{
struct EdgeInsets
{

    static EdgeInsets All(double amount)
    {
        return EdgeInsets({
            .left = amount,
            .right = amount,
            .top = amount,
            .bottom = amount,
        });
    }
    double left = 0.0;
    double right = 0.0;
    double top = 0.0;
    double bottom = 0.0;

    glm::vec4 GetBorders() const
    {
        return glm::vec4(left, right, top, bottom);
    }
};
} // namespace GUI
} // namespace Vultr
