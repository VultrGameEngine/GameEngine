#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <glm/glm.hpp>

namespace Brick3D
{
struct RenderContext
{
    glm::vec2 dimensions;
    glm::vec3 light_position;
    TransformComponent camera_transform;
    CameraComponent camera_component;
    RenderContext(glm::vec2 p_dimensions, glm::vec3 p_light_position,
                  TransformComponent p_camera_transform,
                  CameraComponent p_camera_component)
        : dimensions(p_dimensions), light_position(p_light_position),
          camera_transform(p_camera_transform), camera_component(p_camera_component)
    {
    }
};
}; // namespace Brick3D
