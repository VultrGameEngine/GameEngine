
#pragma once

namespace Vultr
{
    namespace IMGUI
    {
        struct Transform
        {
            Vec2 position = Vec2(0);
            Vec2 scale = Vec2(1);

            Transform operator+(const Transform &other);
        };

        struct RenderTransform
        {
            Vec2 position = Vec2(0);
            Vec2 scale = Vec2(1);

            RenderTransform operator+(const RenderTransform &other);
        };

        Mat4 get_transform_matrix(const RenderTransform &transform);
        // RenderTransform gl_get_transform(const Transform &other);
    } // namespace IMGUI
} // namespace Vultr
