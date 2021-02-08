#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/g_buffer.h>
#include <rendering/models/mesh.h>
#include <rendering/models/render_buffer.h>
#include <rendering/render_group.h>
#include <unordered_map>

const unsigned int GAME = 0;
const unsigned int SCENE = 1;

namespace Brick3D
{
class RenderSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern
    static RenderSystemProvider &Get()
    {
        static RenderSystemProvider instance;
        return instance;
    }

    static glm::vec2 GetDimensions(unsigned int type);
    static void GenerateRenderTexture(FrameBuffer *fbo, Texture *render_texture,
                                      RenderBuffer *rbo, int width, int height);

    std::unordered_map<unsigned int, RenderGroup *> render_groups_deferred;
    std::unordered_map<unsigned int, RenderGroup *> render_groups_forward;

    // GBuffer used for deferred rendering
    GBuffer *g_buffer;

    // Primitives used during deferred rendering
    Mesh *render_quad, render_sphere;

    // Shaders for each of the passes
    Shader *lighting_pass_shader;
    Shader *stencil_pass_shader;
    Shader *composite_pass_shader;

    // Struct containing all of the texture data that will be rendered to
    // Mostly for the editor atm...
    struct RenderTexture
    {
        FrameBuffer *fbo;
        Texture *render_texture;
        RenderBuffer *rbo;
        glm::vec2 dimensions = glm::vec2(1920, 1080);
    } scene, game;

    // Temprorary light stuff for testing
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    glm::vec3 lightPos = glm::vec3(0, 0, 2);
    glm::vec3 attenuation = glm::vec3(1, 0.01f, 0.001f);
    Signature signature;
};

} // namespace Brick3D
