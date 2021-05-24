#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <glm/glm.hpp>

namespace Vultr
{
    struct RenderContext
    {
        glm::vec2 dimensions;
        TransformComponent camera_transform;
        CameraComponent camera_component;
        static void SetContext(glm::vec2 p_dimensions, TransformComponent p_camera_transform, CameraComponent p_camera_component)
        {
            InternalGetContext().dimensions = p_dimensions;
            InternalGetContext().camera_transform = p_camera_transform;
            InternalGetContext().camera_component = p_camera_component;
        }

        static const RenderContext &GetContext()
        {
            return InternalGetContext();
        }

      private:
        RenderContext() : dimensions(glm::vec2(0))
        {
        }
        static RenderContext &InternalGetContext()
        {

            static RenderContext curr_context;
            return curr_context;
        }
    };

}; // namespace Vultr
