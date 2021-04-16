#pragma once
#include <core/components/material_component.h>
#include <rendering/models/mesh.h>
#include <vector>

namespace Vultr::Renderer3D
{

class ForwardRenderer
{
  public:
    // Render a vao and ibo with a shader and transform
    static void Submit(const MaterialComponent &material, const glm::mat4 &transform,
                       const Mesh &mesh);
    static void BindMaterial(const MaterialComponent &material,
                             const glm::mat4 &transform);
};

} // namespace Vultr::Renderer3D
