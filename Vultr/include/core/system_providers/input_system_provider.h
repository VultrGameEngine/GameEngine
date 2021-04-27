#pragma once
#include <ecs/system/system_provider.hpp>
#include <engine.hpp>
#include <GLFW/glfw3.h>
#include <queue>

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
struct ScrollInputEvent
{
    ScrollInputEvent(glm::vec2 p_pos, glm::vec2 p_scroll_amout)
        : scroll_amount(p_scroll_amout), pos(p_pos)
    {
    }
    glm::vec2 pos;
    glm::vec2 scroll_amount;
};

} // namespace Input
class InputSystem;
class InputSystemProvider : public SystemProvider
{
  public:
    static std::shared_ptr<InputSystemProvider> Get()
    {
        return Engine::GetSystemProvider<InputSystemProvider>();
    }

    glm::vec2 MousePosition() const
    {
        return mouse_pos;
    }

    bool MouseDown() const
    {
        return mouse_down;
    }

    glm::vec2 ScrollDelta() const
    {
        return scroll_amount;
    }

    bool KeyDown(const char key) const;

  protected:
    void OnCreateEntity(Entity entity) override;
    void OnDestroyEntity(Entity entity) override;
    std::queue<glm::vec2> scroll_queue;
    void AddScrollInput(glm::vec2 input)
    {
        scroll_queue.push(input);
    }
    GLFWwindow *window;
    glm::vec2 mouse_pos = glm::vec2(0, 0);
    bool mouse_down = false;
    glm::vec2 scroll_amount = glm::vec2(0, 0);
    friend InputSystem;
};

} // namespace Vultr
VultrRegisterSystemProvider(Vultr::InputSystemProvider)
