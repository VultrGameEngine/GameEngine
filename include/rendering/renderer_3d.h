#pragma once
#include "render_context.h"
#include "render_group.h"
#include "render_type.h"
#include "vertex_buffer.h"
#include <array>
#include <rendering/models/g_buffer.h>
#include <unordered_map>

namespace Brick3D
{

class Renderer3D
{

  public:
    static void DeferredGeometryPass(RenderContext context);
    static void LightPass(RenderContext context);
    static void StencilPass(RenderContext context);
    static void ForwardRenderingPass(RenderContext context);
    static void SkyboxPass(RenderContext context);
    static void CompositePass(RenderContext context);
    static void Flush();
};

} // namespace Brick3D
