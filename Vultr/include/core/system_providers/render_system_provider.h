#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/g_buffer.h>
#include <rendering/models/mesh.h>
#include <rendering/models/render_buffer.h>
#include <rendering/render_group.h>
#include <unordered_map>
#include <ecs/world/world.hpp>
#include <helpers/shader_importer.h>
#include <helpers/mesh_importer.h>

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
    RenderSystemProvider()
    {
        GenerateRenderTexture(scene, 1920, 1080);
        GenerateRenderTexture(game, 1920, 1080);
        post_processing_shader = ShaderImporter::ImportShader(
            "/home/brandon/Dev/GameEngine/SandboxTesting/res/shaders/"
            "post_processing.glsl");
        render_quad = MeshImporter::InitQuad();
    }
    // Singleton pattern
    static std::shared_ptr<RenderSystemProvider> Get()
    {
        return World::GetSystemProvider<RenderSystemProvider>();
    }

    static void InitGBuffer(int width, int height);
    static glm::vec2 GetDimensions(unsigned int type);
    static void GenerateRenderTexture(ViewportData &data, int width, int height);
    static void Resize(int width, int height, unsigned int type);

    ViewportData scene;
    ViewportData game;

    Shader *post_processing_shader;
    Mesh *render_quad;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};

} // namespace Vultr
