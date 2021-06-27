#include <gui/core/transform.h>
#include <gui/utils/opengl.h>

using namespace Vultr;

IMGUI::Transform IMGUI::Transform::operator+(const Transform &other)
{
    return {.position = this->position + other.position, .scale = this->scale * other.scale};
}

IMGUI::RenderTransform IMGUI::RenderTransform::operator+(const RenderTransform &other)
{
    return {.position = this->position + other.position, .scale = this->scale * other.scale};
}

Mat4 IMGUI::get_transform_matrix(const RenderTransform &transform)
{
    return glm::translate(Vec3(transform.position, 0)) * glm::scale(Vec3(transform.scale, 1));
}

// IMGUI::RenderTransform IMGUI::gl_get_transform(const Transform &other)
// {
//     auto size = gl_get_size(other.scale);
//     auto pos = gl_get_position(other.position, size);
//     return {.position = pos, .scale = size};
// }
