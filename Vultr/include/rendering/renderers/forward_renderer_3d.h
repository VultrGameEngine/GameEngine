#pragma once
#include <core/components/material_component.h>
#include <rendering/models/mesh.h>

namespace Vultr::Renderer3D
{

    class ForwardRenderer
    {
      public:
        // Render a vao and ibo with a shader and transform
        static void Submit(const MaterialComponent &material, const Mat4 &transform, const Mesh &mesh, const char *skybox_identifier = nullptr);
        static void BindMaterial(const MaterialComponent &material, const Mat4 &transform, const char *skybox_identifier = nullptr);
    };

} // namespace Vultr::Renderer3D
