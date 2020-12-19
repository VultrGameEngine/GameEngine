// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include "../../ecs/system/system.hpp"
#include "../../ecs/world/world.hpp"
#include "../../core/components/transform_component.h"
#include "../../core/components/camera_component.h"
#include <glm/glm.hpp>

const unsigned int GAME = 0;
const unsigned int SCENE = 1;

class RenderSystem : public System
{
public:
    static std::shared_ptr<RenderSystem> Get();
    void Update(float delta_time);
    void DestroyEntity(Entity entity) override;
    static std::shared_ptr<RenderSystem> RegisterSystem();
    static void Resize(int width, int height, unsigned int type);
    static glm::vec2 GetDimensions(unsigned int type);
    struct RenderTexture
    {
        unsigned int fbo;
        unsigned int render_texture;
        unsigned int rbo;
        glm::vec2 dimensions = glm::vec2(1920, 1080);
    } scene, game;

private:
    void RenderElements(unsigned int type, TransformComponent camera_transform, CameraComponent camera_component, Entity light);
    void RenderSkybox(unsigned int type, TransformComponent camera_transform, CameraComponent camera_component);
    static void GenerateRenderTexture(unsigned int *fbo, unsigned int *render_texture, unsigned int *rbo, int width, int height);
};