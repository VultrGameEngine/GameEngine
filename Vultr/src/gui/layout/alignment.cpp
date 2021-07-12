#include <gui/layout/alignment.h>

using namespace Vultr::IMGUI;

Alignment Alignment::BOTTOM_CENTER()
{
    return {.value = Vec2(0, 1)};
}

Alignment Alignment::BOTTOM_LEFT()
{
    return {.value = Vec2(-1, 1)};
}

Alignment Alignment::BOTTOM_RIGHT()
{
    return {.value = Vec2(1, 1)};
}

Alignment Alignment::CENTER()
{
    return {.value = Vec2(0, 0)};
}

Alignment Alignment::CENTER_LEFT()
{
    return {.value = Vec2(-1, 0)};
}

Alignment Alignment::CENTER_RIGHT()
{
    return {.value = Vec2(1, 0)};
}

Alignment Alignment::TOP_CENTER()
{
    return {.value = Vec2(0, -1)};
}

Alignment Alignment::TOP_LEFT()
{
    return {.value = Vec2(-1, -1)};
}

Alignment Alignment::TOP_RIGHT()
{
    return {.value = Vec2(1, -1)};
}
