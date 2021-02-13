#pragma once
#include <core/components/material_component.h>
#include <rendering/models/mesh.h>
#include <vector>

namespace Brick3D::Renderer3D
{

class ForwardRenderer
{
  public:
    // Render a vao and ibo with a shader and transform
    static void Submit(const MaterialComponent &material, const glm::mat4 &transform,
                       const Mesh &mesh);
};

} // namespace Brick3D::Renderer3D
