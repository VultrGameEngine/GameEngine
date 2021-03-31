#pragma once
#include <ecs/system/system_provider.hpp>
#include <ecs/world/world.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Vultr
{
namespace Input
{
struct MouseInputEvent
{
    MouseInputEvent(glm::vec2 p_pos, bool p_mouse_down)
        : pos(p_pos), mouse_down(p_mouse_down)
    {
    }
    glm::vec2 pos;
    bool mouse_down;
};

struct MouseButtonInputEvent
{
    MouseButtonInputEvent(glm::vec2 p_pos, bool p_mouse_down)
        : pos(p_pos), mouse_down(p_mouse_down)
    {
    }
    glm::vec2 pos;
    bool mouse_down;
};
} // namespace Input
class InputSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<InputSystemProvider> Get()
    {
        return World::GetSystemProvider<InputSystemProvider>();
    }

    glm::vec2 mouse_pos = glm::vec2(0, 0);
    bool mouse_down = false;
    GLFWwindow *window;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
};

} // namespace Vultr
