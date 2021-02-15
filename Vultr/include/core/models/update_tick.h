#pragma once

namespace Brick3D
{
struct UpdateTick
{
    UpdateTick(float p_delta_time, bool p_debug)
        : debug(p_debug), m_delta_time(p_delta_time)
    {
    }

    float m_delta_time;
    bool debug = false;
};

} // namespace Brick3D
