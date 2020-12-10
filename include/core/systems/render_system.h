// The Render System will loop through all entities with the renderer component
// The render component holds the ids for all of the buffers
// The render system will bind these ids and render them via opengl

#pragma once
#include "../../ecs/system/system.hpp"
#include "../../ecs/coordinator/coordinator.hpp"

class RenderSystem : public System
{
public:
    static std::shared_ptr<RenderSystem> Get();
    void Update(float delta_time);
    void DestroyEntity(Entity entity) override;
    static std::shared_ptr<RenderSystem> RegisterSystem();

private:
    static std::shared_ptr<RenderSystem> instance;
    static Signature signature;
};