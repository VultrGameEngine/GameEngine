#pragma once
#include <rendering/types/mesh.h>
#include <rendering/types/texture.h>
#include <rendering/render_context.h>
#include <rendering/render_type.h>

namespace Vultr
{
    struct RenderEntity
    {
        Mesh *mesh;
        Shader *shader;
        // TODO: Reimplement??
        // std::vector<Texture> textures;
    };

    class RenderGroup
    {
      public:
        RenderGroup(Shader *input_shader) : shader(input_shader)
        {
        }

        ~RenderGroup()
        {
            delete_shader(shader);
        }

        void Render(RenderContext context, RenderType type);
        void RegisterEntity(Mesh *mesh, const std::vector<Texture *> &textures);

      private:
        std::set<RenderEntity> entities;
        Shader *shader;
    };
} // namespace Vultr
