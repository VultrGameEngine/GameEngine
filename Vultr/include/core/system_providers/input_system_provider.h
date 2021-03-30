#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vultr
{
class InputSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<InputSystemProvider> Get()
    {
        return World::GetSystemProvider<InputSystemProvider>();
    }

    glm::vec2 mouse_pos = glm::vec2(0, 0);
    GLFWwindow *window;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};

} // namespace Vultr
