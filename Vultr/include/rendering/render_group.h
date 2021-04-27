#pragma once
#include <rendering/models/mesh.h>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>
#include <rendering/render_type.h>

namespace Vultr
{
struct RenderEntity
{
    Mesh *mesh;
    Shader *shader;
    std::vector<Texture *> textures;
};

class RenderGroup
{
  public:
    RenderGroup(Shader *input_shader) : shader(input_shader)
    {
    }

    ~RenderGroup()
    {
        delete shader;
    }

    void Render(RenderContext context, RenderType type);
    void RegisterEntity(Mesh *mesh, const std::vector<Texture *> &textures);

  private:
    std::set<RenderEntity> entities;
    Shader *shader;
};
} // namespace Vultr
