#pragma once
#include <filesystem/virtual_filesystem.h>
#include <rendering/types/shader.h>
#include <rendering/types/texture.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
#define MAX_MESHES 1024
#define MAX_SHADERS 512
#define MAX_TEXTURES 2048
    struct InternalRenderContext;
    typedef InternalRenderContext RenderContext;

    Shader *rc_get_shader(RenderContext *context, VFileHandle *res);
    Texture *rc_get_texture(RenderContext *context, VFileHandle *res);
    Mesh *rc_get_mesh(RenderContext *context, VFileHandle *res);
}; // namespace Vultr
