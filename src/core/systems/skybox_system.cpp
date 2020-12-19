#include "../../../include/core/components/sky_box_component.h"
#include "../../../include/core/components/texture_cache.h"
#include "../../../include/core/components/shader_component.h"
#include "../../../include/core/systems/skybox_system.h"
#include "../../../include/core/systems/camera_system.h"
#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/world/world.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void SkyboxSystem::Update()
{
}

std::shared_ptr<SkyboxSystem> SkyboxSystem::Get()
{
    static std::shared_ptr<SkyboxSystem> instance;
    if (instance == nullptr)
    {
        instance = RegisterSystem();
    }
    return instance;
}

std::shared_ptr<SkyboxSystem> SkyboxSystem::RegisterSystem()
{
    std::shared_ptr<SkyboxSystem> ptr = World::RegisterSystem<SkyboxSystem>();
    return ptr;
}
