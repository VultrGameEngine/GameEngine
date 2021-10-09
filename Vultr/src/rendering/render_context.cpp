#include <rendering/render_context.h>
#include <rendering/internal/internal_render_context.h>
#include <rendering/types/internal/internal_shader.h>
#include <rendering/types/internal/internal_texture.h>

namespace Vultr
{
    InternalRenderContext::InternalRenderContext()
    {
    }
    RenderContext::~InternalRenderContext()
    {
    }

    Shader *rc_get_shader(RenderContext *context, VFileHandle *res)
    {
    }

    Texture *rc_get_texture(RenderContext *context, VFileHandle *res)
    {
    }

    Mesh *rc_get_mesh(RenderContext *context, VFileHandle *res)
    {
    }
} // namespace Vultr
