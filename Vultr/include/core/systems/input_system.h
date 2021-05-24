#pragma once

struct UpdateTick;
namespace Vultr::Input
{
    struct MouseInputEvent
    {
        MouseInputEvent(glm::vec2 p_pos, bool p_mouse_down) : pos(p_pos), mouse_down(p_mouse_down)
        {
        }
        glm::vec2 pos;
        bool mouse_down;
    };

    struct MouseButtonInputEvent
    {
        MouseButtonInputEvent(glm::vec2 p_pos, bool p_mouse_down) : pos(p_pos), mouse_down(p_mouse_down)
        {
        }
        glm::vec2 pos;
        bool mouse_down;
    };

    struct ScrollInputEvent
    {
        ScrollInputEvent(glm::vec2 p_pos, glm::vec2 p_scroll_amout) : scroll_amount(p_scroll_amout), pos(p_pos)
        {
        }
        glm::vec2 pos;
        glm::vec2 scroll_amount;
    };

} // namespace Vultr::Input

namespace Vultr::InputSystem
{
    typedef void (*MouseClickEvent)(int button);
    typedef void (*KeyPressEvent)(int key, int scancode, int action, int mods);
    void register_system();
    void init(GLFWwindow *window);
    void update(const UpdateTick &tick);
    void on_mouse_click(const char *name, MouseClickEvent event);
    void on_key_press(const char *name, KeyPressEvent event);

} // namespace Vultr::InputSystem
