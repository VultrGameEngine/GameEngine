// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include "../../ecs/system/system.hpp"
#include "../../ecs/world/world.hpp"
#include <glm/glm.hpp>

class RenderSystem : public System
{
public:
    static std::shared_ptr<RenderSystem> Get();
    void Update(float delta_time);
    void DestroyEntity(Entity entity) override;
    static std::shared_ptr<RenderSystem> RegisterSystem();
    static void Resize(int width, int height);
    static glm::vec2 GetDimensions();
    unsigned int fbo;
    unsigned int render_texture;
    unsigned int rbo;

private:
    glm::vec2 dimensions = glm::vec2(1920, 1080);
};