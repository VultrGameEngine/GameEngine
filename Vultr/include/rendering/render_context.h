#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <glm/glm.hpp>

namespace Vultr
{
struct RenderContext
{
    glm::vec2 dimensions;
    glm::vec3 light_position;
    TransformComponent camera_transform;
    CameraComponent camera_component;
    static void SetContext(glm::vec2 p_dimensions, glm::vec3 p_light_position,
                           TransformComponent p_camera_transform,
                           CameraComponent p_camera_component)
    {
        InternalGetContext().dimensions = p_dimensions;
        InternalGetContext().light_position = p_light_position;
        InternalGetContext().camera_transform = p_camera_transform;
        InternalGetContext().camera_component = p_camera_component;
    }

    static const RenderContext &GetContext()
    {
        return InternalGetContext();
    }

  private:
      RenderContext() : dimensions(glm::vec2(0)), light_position(glm::vec3(0))
    {
    }
    static RenderContext &InternalGetContext()
    {

        static RenderContext curr_context;
        return curr_context;
    }
};

}; // namespace Vultr
