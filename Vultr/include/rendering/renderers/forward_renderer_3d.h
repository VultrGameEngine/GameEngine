#pragma once
#include <engine.hpp>
#include <core/components/material_component.h>
#include <rendering/types/mesh.h>
#include <rendering/render_context.h>

namespace Vultr::Renderer3D
{
    namespace ForwardRenderer
    {
        // Render a vao and ibo with a shader and transform
        void submit(Engine *e, const MaterialComponent &material, const Mat4 &transform, const Mesh &mesh, const RenderContext &context, u32 skybox_identifier = 0);
        void bind_material(Engine *e, const MaterialComponent &material, const Mat4 &transform, const RenderContext &context, u32 skybox_identifier = 0);
    }; // namespace ForwardRenderer

} // namespace Vultr::Renderer3D
