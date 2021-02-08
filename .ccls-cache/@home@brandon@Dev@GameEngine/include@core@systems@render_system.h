// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include <core/components/camera_component.h>
#include <core/components/transform_component.h>
#include <core/models/update_tick.h>
#include <core/system_providers/render_system_provider.h>
#include <ecs/system/system.hpp>
#include <ecs/world/world.hpp>
#include <glm/glm.hpp>
#include <rendering/renderer_3d.h>

const unsigned int GAME = 0;
const unsigned int SCENE = 1;

namespace Brick3D
{
class RenderSystem : public System
{
  public:
    void Update(UpdateTick meta_data);
    static std::shared_ptr<RenderSystem> RegisterSystem();
    static void Resize(int width, int height, unsigned int type);
    struct RenderTexture
    {
        unsigned int fbo;
        unsigned int render_texture;
        unsigned int rbo;
        glm::vec2 dimensions = glm::vec2(1920, 1080);
    } scene, game;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
    SystemProvider &GetProvider() override
    {
        return RenderSystemProvider::Get();
    }

  private:
    void RenderElements(unsigned int type, TransformComponent camera_transform,
                        CameraComponent camera_component, Entity light);
    void RenderSkybox(unsigned int type, TransformComponent camera_transform,
                      CameraComponent camera_component);
    static void GenerateRenderTexture(unsigned int *fbo,
                                      unsigned int *render_texture,
                                      unsigned int *rbo, int width, int height);
    Signature signature;
    Renderer3D *renderer;
};
} // namespace Brick3D
