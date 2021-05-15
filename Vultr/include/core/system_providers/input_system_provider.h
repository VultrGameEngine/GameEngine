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

    } // namespace Input

    namespace InputSystem
    {
        struct Component : public SystemProvider
        {
            std::queue<glm::vec2> scroll_queue;
            GLFWwindow *window;
            glm::vec2 mouse_pos = glm::vec2(0, 0);
            glm::vec2 scene_mouse_pos = glm::vec2(0, 0);
            glm::vec2 scroll_amount = glm::vec2(0, 0);
            bool mouse_down = false;
        };

        Component &get_provider();

        bool key_down(const char key);
        void add_scroll_input(glm::vec2 input);
    } // namespace InputSystem
    template <>
    inline const char *get_struct_name<InputSystem::Component>()
    {
        return "InputSystem";
    }

} // namespace Vultr
// VultrRegisterSystemProvider(Vultr::InputSystemProvider)
