#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ecs/entity/entity.hpp>
#include <ecs/world/world.hpp>
#include <memory>
#include <rendering/models/mesh.h>
#include <rendering/models/texture.h>
#include <rendering/render_context.h>
#include <rendering/render_type.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace Brick3D
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
} // namespace Brick3D
