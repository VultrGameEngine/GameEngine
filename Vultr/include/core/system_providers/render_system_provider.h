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
#include <GLFW/glfw3.h>
#include <engine.hpp>

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
    glm::vec2 position = glm::vec2(0);
};
class RenderSystem;
class RenderSystemProvider : public SystemProvider
{
  public:
    RenderSystemProvider()
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        GenerateRenderTexture(scene, mode->width, mode->height);
        GenerateRenderTexture(game, mode->width, mode->height);
        post_processing_shader =
            ShaderImporter::ImportShader("res/shaders/post_processing.glsl");
        render_quad = MeshImporter::InitQuad();
        m_camera_mesh = MeshImporter::ImportMesh("res/models/editor/Camera.blend");
        camera_mat = Vultr::ForwardMaterial::Create("res/textures/cube/blank.jpg");
    }
    // Singleton pattern
    static std::shared_ptr<RenderSystemProvider> Get()
    {
        return Engine::GetSystemProvider<RenderSystemProvider>();
    }

    static void InitGBuffer(int width, int height);
    static glm::vec2 GetDimensions(unsigned int type);
    static void GenerateRenderTexture(ViewportData &data, int width, int height);
    static void Resize(int width, int height, unsigned int type);
    static void UpdateViewportPos(int x, int y, unsigned int type);

    ViewportData scene;
    ViewportData game;

    Shader *post_processing_shader;
    Mesh *render_quad;

    template <class Archive> void serialize(Archive &ar)
    {
        // We pass this cast to the base type for each base type we
        // need to serialize.  Do this instead of calling serialize functions
        // directly
        ar(cereal::base_class<SystemProvider>(this));
    }

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
    Mesh *m_camera_mesh;
    MaterialComponent camera_mat;
    friend RenderSystem;
};

} // namespace Vultr
VultrRegisterSystemProvider(Vultr::RenderSystemProvider)
