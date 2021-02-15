#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/g_buffer.h>
#include <rendering/models/mesh.h>
#include <rendering/models/render_buffer.h>
#include <rendering/render_group.h>
#include <unordered_map>

const unsigned int GAME = 0;
const unsigned int SCENE = 1;

namespace Vultr
{
// Struct containing all of the texture data that will be rendered to
// Mostly for the editor atm...
struct ViewportData
{
    FrameBuffer *fbo = nullptr;
    Texture *render_texture = nullptr;
    RenderBuffer *rbo = nullptr;
    glm::vec2 dimensions = glm::vec2(1920, 1080);
};
class RenderSystemProvider : public SystemProvider
{
  public:
    // Singleton pattern
    static RenderSystemProvider &Get()
    {
        static RenderSystemProvider instance = RenderSystemProvider();
        return instance;
    }

    static void InitGBuffer(int width, int height);
    static glm::vec2 GetDimensions(unsigned int type);
    static void GenerateRenderTexture(ViewportData &data, int width, int height);
    static void Resize(int width, int height, unsigned int type);

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

    ViewportData scene;
    ViewportData game;

    // Temprorary light stuff for testing
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    glm::vec3 lightPos = glm::vec3(0, 0, 2);
    glm::vec3 attenuation = glm::vec3(1, 0.01f, 0.001f);

  private:
    RenderSystemProvider()
    {
        GenerateRenderTexture(scene, 1920, 1080);
        GenerateRenderTexture(game, 1920, 1080);
    }
};

} // namespace Vultr
